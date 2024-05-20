# train mobilenetv3 small for esp32

import tensorflow as tf
from tensorflow.keras.applications import MobileNetV3Small
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D, Conv2D, ReLU
from tensorflow.keras.models import Model
from tensorflow.keras.preprocessing.image import ImageDataGenerator

from tensorflow.keras.callbacks import EarlyStopping, ReduceLROnPlateau

DATASET_PATH = './animals'
NAME_PATH = './animal_names.txt'
EPOCHS = 100
INPUT_SHAPE = (224, 224, 3)

names = []
with open(NAME_PATH, 'r') as f:
    for line in f:
        names.append(line.strip())

N_CLASSES = len(names)

# create the model with weights
base_model = MobileNetV3Small(weights='imagenet', include_top=False, input_shape=INPUT_SHAPE)
base_model.trainable = False
x = base_model.output
x = GlobalAveragePooling2D()(x)
x = Dense(420, activation='relu')(x)
predictions = Dense(N_CLASSES, activation='softmax')(x)

# create the full model
base_model = Model(inputs=base_model.input, outputs=predictions)

# Training parameters
learning_rate = 0.0001
batch_size = 32

optimizer = tf.keras.optimizers.Adam(learning_rate=learning_rate)

# Compile the model
base_model.compile(optimizer=optimizer, loss='categorical_crossentropy', metrics=['accuracy'])

# Prepare the data
train_datagen = ImageDataGenerator(
    rescale=1./255,
    validation_split=0.2,
    rotation_range=20,
    width_shift_range=0.2,
    height_shift_range=0.2,
    horizontal_flip=True,
    vertical_flip=True,
    zoom_range=0.2
)

train_generator = train_datagen.flow_from_directory(
    DATASET_PATH,
    target_size=INPUT_SHAPE[:2],
    batch_size=32,
    class_mode='categorical',
    subset='training',
    shuffle=True
)

validation_generator = train_datagen.flow_from_directory(
    DATASET_PATH,
    target_size=INPUT_SHAPE[:2],
    batch_size=32,
    class_mode='categorical',
    subset='validation'
)

# Callbacks
early_stopping = EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)
reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=5, min_lr=0.00001)

# print current device
print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))

# Train the model
base_model.fit(
    train_generator,
    steps_per_epoch=train_generator.samples // train_generator.batch_size,
    validation_data=validation_generator,
    validation_steps=validation_generator.samples // validation_generator.batch_size,
    epochs=EPOCHS,
    callbacks=[early_stopping, reduce_lr]
)

# Save the model
base_model.save('mobilenetv3_small.h5')

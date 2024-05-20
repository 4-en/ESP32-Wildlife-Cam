# train mobile net for image classification on esp32

import tensorflow as tf
from tensorflow.keras.applications.mobilenet import MobileNet, preprocess_input, decode_predictions
from tensorflow.keras.preprocessing import image
import numpy as np

# Load the MobileNet model pre-trained on ImageNet
model = MobileNet(weights='imagenet')

# example image
image_path = "./cat.jpg"

# Load the image with the correct target size for MobileNet
img = image.load_img(image_path, target_size=(224, 224))

# Convert the image to a numpy array
img_array = image.img_to_array(img)

# Add a batch dimension
img_array = np.expand_dims(img_array, axis=0)

# Preprocess the image for the MobileNet model
img_array = preprocess_input(img_array)

# Make predictions
predictions = model.predict(img_array)

# Decode the predictions
decoded_predictions = decode_predictions(predictions, top=3)[0]

# Print the top-3 predictions
for i, (imagenet_id, label, score) in enumerate(decoded_predictions):
    print(f'{i+1}: {label} ({score:.2f})')

print("Model ran successfully!")

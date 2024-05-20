import tensorflow as tf
from tensorflow.keras.applications.inception_v3 import InceptionV3, preprocess_input, decode_predictions
from tensorflow.keras.preprocessing import image
import numpy as np

# Load the InceptionV3 model pre-trained on ImageNet
model = InceptionV3(weights='imagenet')

# Download an example image from the internet
image_path = "./cat.jpg"

# Load the image with the correct target size for InceptionV3
img = image.load_img(image_path, target_size=(299, 299))

# Convert the image to a numpy array
img_array = image.img_to_array(img)

# Add a batch dimension
img_array = np.expand_dims(img_array, axis=0)

# Preprocess the image for the InceptionV3 model
img_array = preprocess_input(img_array)

# Make predictions
predictions = model.predict(img_array)

# Decode the predictions
decoded_predictions = decode_predictions(predictions, top=3)[0]

# Print the top-3 predictions
for i, (imagenet_id, label, score) in enumerate(decoded_predictions):
    print(f'{i+1}: {label} ({score:.2f})')

print("Model ran successfully!")
import tensorflow as tf

# Load the saved Keras model
model = tf.keras.models.load_model('mobilenet_v2_model.h5')

# Convert the model to TensorFlow Lite format with quantization
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_quant_model = converter.convert()

# Save the quantized model to a file
with open('mobilenet_v2_model_quant.tflite', 'wb') as f:
    f.write(tflite_quant_model)

print("Quantized model size:", len(tflite_quant_model))
# server.py
# test server for esp32 cam
# receive image from esp32 cam and show it

from fastapi import FastAPI, Request
from pydantic import BaseModel
import cv2
import numpy as np
import os

app = FastAPI()

UPLOAD_DIR = "uploads"
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)


@app.post("/showimage/")
async def show_image(image: Request):
    data: bytes = await image.body()

    # save jpg file
    with open("uploads/test.jpg", "wb") as f:
        f.write(data)

    # read jpg file
    img = cv2.imread("uploads/test.jpg")
    img_np = np.array(img)
    cv2.imshow("image", img_np)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return {"message": "image received"}

class PrintPostMessage(BaseModel):
    message: str

@app.post("/printmessage/")
async def print_message(message: PrintPostMessage):
    print(message.message)
    return {"message": message.message}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)

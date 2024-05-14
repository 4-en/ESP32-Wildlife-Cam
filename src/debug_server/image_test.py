# test server for esp32 cam
# receive image from esp32 cam and show it

from fastapi import FastAPI, File, UploadFile
# request models
from pydantic import BaseModel
import cv2

app = FastAPI()

@app.post("/uploadfile/")
async def create_upload_file(file: UploadFile = File(...)):
    with open(file.filename, "wb") as f:
        f.write(file.file.read())
    return {"filename": file.filename}

@app.post("/showimage/")
async def show_image(file: UploadFile = File(...)):
    with open(file.filename, "wb") as f:
        f.write(file.file.read())
    img = cv2.imread(file.filename)
    cv2.imshow('image', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return {"filename": file.filename}

class PrintPostMessage(BaseModel):
    message: str

@app.post("/printmessage/")
async def print_message(message: PrintPostMessage):
    print(message.message)
    return {"message": message.message}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)



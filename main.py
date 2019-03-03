import cv2
import sys
import serial
import time
from gtts import gTTS
from pygame import mixer

ser = serial.Serial('COM3', 9600)

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(1)
buffer = []

startingTime = 0
currentTime = 0

tts = gTTS('wake up')
tts.save("ringer.mp3")
mixer.init()
mixer.music.load("ringer.mp3")

while True:
    currentTime = time.clock()
    # Capture frame-by-frame
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30),
        flags=cv2.CASCADE_SCALE_IMAGE
    )

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    # Display the resulting frame
    cv2.imshow('Video', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    buffer.append(len(faces) >= 1)
    if len(buffer) > 10:
        buffer.pop(0)

    if (len(buffer) == 10 and all(buffer)):
        if startingTime == 0:
            mixer.music.play()
            startingTime = time.clock()
        elif currentTime - startingTime > 15 and currentTime - startingTime <= 30:
            print("sending 1")
            ser.write("1".encode('utf-8'))
        elif currentTime - startingTime > 30:
            print("sending 2")
            ser.write("2".encode('utf-8'))

    else:
        print("nope")
        ser.write("0".encode('utf-8'))

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()

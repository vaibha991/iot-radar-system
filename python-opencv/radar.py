import cv2
import numpy as np
import serial
import math

# change COM3 to whatever port your Arduino is on
# check Device Manager if you're not sure
ser = serial.Serial('COM3', 115200)

width, height = 800, 600

while True:
    img = np.zeros((height, width, 3), dtype=np.uint8)

    center_x = width // 2
    center_y = height - 50

    # draw range rings every 100px
    for r in range(100, 400, 100):
        cv2.circle(img, (center_x, center_y), r, (0, 255, 0), 1)

    # draw angle lines across 180 degrees
    for angle in range(0, 181, 30):
        rad = math.radians(angle)
        x = int(center_x + 400 * math.cos(rad))
        y = int(center_y - 400 * math.sin(rad))
        cv2.line(img, (center_x, center_y), (x, y), (0, 100, 0), 1)

    # read from serial if data is waiting
    if ser.in_waiting > 0:
        try:
            data = ser.readline().decode().strip()

            # Arduino sends: angle,distance
            angle, distance = data.split(',')
            angle = int(angle)
            distance = int(distance)

            rad = math.radians(angle)

            # where the object actually is
            x = int(center_x + distance * math.cos(rad))
            y = int(center_y - distance * math.sin(rad))
            cv2.circle(img, (x, y), 6, (0, 0, 255), -1)

            # green scanning line at current angle
            scan_x = int(center_x + 400 * math.cos(rad))
            scan_y = int(center_y - 400 * math.sin(rad))
            cv2.line(img, (center_x, center_y), (scan_x, scan_y), (0, 255, 0), 2)

        except:
            pass  # bad/incomplete serial data, just skip it

    cv2.imshow("RADAR SYSTEM", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

ser.close()
cv2.destroyAllWindows()
import cv2

def main():
    # Create a VideoCapture object to read from the camera
    cap = cv2.VideoCapture(0)

    # Check if the camera is opened successfully
    if not cap.isOpened():
        print("Failed to open camera")
        return

    while True:
        # Read a frame from the camera
        ret, frame = cap.read()

        # If the frame was not captured successfully, break the loop
        if not ret:
            break

        # Display the frame
        cv2.imshow("USB Camera", frame)

        # Wait for the 'q' key to be pressed to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the VideoCapture and close the OpenCV windows
    cap.release()

if __name__ == "__main__":
    main()

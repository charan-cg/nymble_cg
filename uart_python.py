import serial
import time
import sys

def main():
    ser = serial.Serial('COM8', baudrate=2400, timeout=2)
    text_data = (
        "Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that "
        "the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action "
        "when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of "
        "2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying "
        "that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, "
        "not just the last one. In particular, government should refrain from setting ambitious credit targets or waiving "
        "loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment "
        "for future NPAs,\" Rajan said in the note.\" Both MUDRA loans as well as the Kisan Credit Card, while popular, "
        "have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till "
        "September 2016, is currently."
    ).encode('utf-8')
    print(text_data)
    print("Sending data...")
    ser.write(text_data)
    ser.write(b'\n')
    print("Data sent, waiting for response...")
    sent_data = ser.write(text_data)
    print("Sent data = ", sent_data)

    start_time = time.time()
    received_data = b""
    
    while True:
        byte = ser.read(1)
        if byte:
            received_data += byte
            elapsed_time = time.time() - start_time
            transmission_speed = len(received_data) * 8 / elapsed_time
            
            print(f"Current Transmission Speed: {transmission_speed:.2f} bps")
            print(f"Received Byte: {byte.decode('utf-8', errors='ignore')}\n", end='')
            
            if byte == b'\n':
                break

        if (time.time() - start_time) > 10:
            print("Timeout waiting for response.")
            break
            
    full_data_str = received_data.decode('utf-8', errors='ignore')
    print("\nFull Received Data:", full_data_str)
    print("Length of Received Data:", len(full_data_str))
    print("size of Received Data:", sys.getsizeof(full_data_str))
    ser.close()

if __name__ == "__main__":
    main()
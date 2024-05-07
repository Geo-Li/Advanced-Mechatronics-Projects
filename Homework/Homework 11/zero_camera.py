from time import sleep
from picamera import PiCamera

CONFIRMATION = ["Y", "YES"]


def take_picture(color=True):
    iso_settings = [100,200,400,800,1600]
    iso_ui = "Please select the iso for photography:\n"
    iso_ui += "\t0: 100\n"
    iso_ui += "\t1: 200\n"
    iso_ui += "\t2: 400\n"
    iso_ui += "\t3: 800\n"
    iso_ui += "\t4: 1600\n"
    pic_confirm = "Do you want to take a picture? [Y/N]\n"
    with PiCamera(resolution=(1920, 1080), framerate=30) as camera:
        if not color:
            camera.color_effects = (128,128)
            taking_pic = input(pic_confirm)
        while taking_pic.upper() in CONFIRMATION:
            if not color:
                iso_index = int(input(iso_ui))
                camera.iso = iso_settings[iso_index]
            print("Taking picture...")
            camera.capture("picture.jpg")
            print("Picture taken!")
            taking_pic = input(pic_confirm)



def main() -> int:
    is_color = input("Do you want to take colored pictures? [Y/N]\n")
    if is_color.upper() in CONFIRMATION:
        color=True
    else:
        color=False
    take_picture(color)
    return 0



if __name__ == "__main__":
    main()

import math
import time 
  
# a module which has functions related to time.  
# It can be installed using cmd command:  
# pip install time, in the same way as pyautogui. 
import pyautogui 
# time.sleep(10)  
  
time.sleep(10)
sleep_time = 0.1

x_center, y_center = pyautogui.position()

#outer_radius = 320 # osc knobs 2300%zoom?
outer_radius = 350
inner_radius = 100

#nr_stripes = 15 #osc knobs
nr_stripes = 21 #big knobs

make_between = False
between_radius = 200

start_angle = -130
end_angle = 130

pyautogui.press('b')
time.sleep(sleep_time)
time.sleep(sleep_time)

for phi in range(0, nr_stripes):
    pyautogui.moveTo(x_center + inner_radius * math.sin(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))), y_center - inner_radius * math.cos(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))))
    pyautogui.click()

    time.sleep(sleep_time)

    pyautogui.moveTo(x_center + outer_radius * math.sin(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))), y_center - outer_radius * math.cos(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))))
    pyautogui.click()

    time.sleep(sleep_time)

    pyautogui.press('m')
    time.sleep(sleep_time)
    
    pyautogui.press('b')
    time.sleep(sleep_time)
    
if make_between:
    half_step = + 0.5 / (nr_stripes - 1)* (end_angle - start_angle)

    start_angle = start_angle + half_step
    end_angle = end_angle - half_step
    nr_stripes = nr_stripes - 1

    for phi in range(0, nr_stripes):
        pyautogui.moveTo(x_center + inner_radius * math.sin(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))), y_center - inner_radius * math.cos(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))))
        pyautogui.click()

        time.sleep(sleep_time)

        pyautogui.moveTo(x_center + between_radius * math.sin(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))), y_center - between_radius * math.cos(math.radians(start_angle + phi / (nr_stripes - 1)* (end_angle - start_angle))))
        pyautogui.click()

        time.sleep(sleep_time)

        pyautogui.press('m')
        time.sleep(sleep_time)
    
        pyautogui.press('b')
        time.sleep(sleep_time)
#!/usr/bin/env python

#coding=utf-8
from pyvirtualdisplay import Display
from selenium import webdriver
def get1(sr_url):
    display = Display(visible=0, size=(800,600))
    display.start()
    profile = webdriver.FirefoxProfile()
    profile.set_preference('permissions.default.stylesheet',2)
    profile.set_preference('permission.default.image',2)
    profile.set_preference('dom.ipc.plugins.enabled.libflashplay.so', 'false')

    browser = webdriver.Firefox(firefox_profile=profile)
    browser.get(sr_url)
    final_url = browser.current_url
    browser.quit()
    display.stop()

    return final_url

if __name__ == '__main__':
    import sys
    for line in sys.stdin:
        print get1(line.strip())

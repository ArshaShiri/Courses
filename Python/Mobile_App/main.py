from re import L
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.screenmanager import ScreenManager, Screen
from hoverable import HoverBehavior
from kivy.uix.image import Image
from kivy.uix.behaviors import ButtonBehavior
import json
import datetime
import glob
from pathlib import Path
import random

Builder.load_file("design.kv")

class LoginScreen(Screen):
    def signUp(self):
        self.manager.current = "signUpScreen"
    
    def login(self, username, password):
        with open("users.json") as file:
            users = json.load(file)
        
        if username in users and users[username]["password"] == password:
            self.manager.current = "loginScreenSuccess"
        else:
            self.ids.wrongCredentials.text = "Wrong username or password!"             


class SignUpScreen(Screen):
    def addUser(self, username, password):
        with open("users.json") as file:
            users = json.load(file)
        
        users[username] = {"username": username, 
                           "password": password,
                           "created": datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S")}
        
        with open("users.json", "w") as file:
            json.dump(users, file)
        
        self.manager.current = "signUpScreenSuccess"

class SignUpScreenSuccess(Screen):
    def goToLogin(self):
        self.manager.transition.direction = "right"
        self.manager.current = "loginScreen"

class LoginScreenSuccess(Screen):
    def logout(self):
        print("Logout!!")
        self.manager.transition.direction = "right"
        self.manager.current = "loginScreen"
    def getQuote(self, feeling):
        feeling = feeling.lower()
        availableFeelings = glob.glob("quotes/*txt")
        availableFeelings = [Path(fileName).stem for fileName in availableFeelings]

        if feeling in availableFeelings:
            with open(f"quotes/{feeling}.txt") as file:
                quotes = file.readlines()
            
            self.ids.quote.text = random.choice(quotes)
        else:
            self.ids.quote.text = "Try another feeling!"

class ImageButton(ButtonBehavior, HoverBehavior, Image):
    pass            

class RootWidget(ScreenManager):
    pass

class MainApp(App):
    def build(self):
        return RootWidget()

if __name__ == "__main__":
    MainApp().run()
from tkinter import *
from PIL import ImageTk, Image #(z.B. fÃ¼r ein Icon (Logo HSO))
import random
import threading
import paho.mqtt.client as mqtt_client

#MQTT
#broker = "127.0.0.1"
broker = "192.168.137.1"
port = 1883
client_id = f'subscribe-{random.randint(0, 100)}'

topic_rot = "button_rot"
topic_blau = "button_blau"
topic_weiss = "button_weiss"

gk=0
kk=0
cap=0
lm=0
ms=0
hw=0

choice = None
topic_kaffee = "kaffeesorten"

#Index fŸr die Liste
index=-1

#Definition €nderungen Design bei Auswahl
def reset_choice():
    global choice
    kaffee_klein_btn.configure(bg='white')
    kaffee_gross_btn.configure(bg='white')
    cappuccino_btn.configure(bg='white')
    latte_btn.configure(bg='white')
    milchschaum_btn.configure(bg='white')
    wasser_btn.configure(bg='white')
    choice = None
    

def wahl_kk():
    global choice
    reset_choice()
    kaffee_klein_btn.configure(bg='red')
    choice = "Small Coffee"
    
    
def wahl_gk():
    global choice
    reset_choice()
    kaffee_gross_btn.configure(bg='red')
    choice = "Big Coffee"
    
   
def wahl_cap():
    global choice
    reset_choice()
    cappuccino_btn.configure(bg='red')
    choice = "Cappucino"
    
   
def wahl_lm():
    global choice
    reset_choice()
    latte_btn.configure(bg='red')
    choice = "Latte Macchiato"
    
   
def wahl_ms():
    global choice
    reset_choice()
    milchschaum_btn.configure(bg='red')
    choice = "Milk Foam"
    
   
def wahl_hw():
    global choice
    reset_choice()
    wasser_btn.configure(bg='red')
    choice = "Hot Water"
    
   
def reset_menu():
    reset_choice()
    

def publish_msg(client, coffee_str):
    client.publish(topic_kaffee, coffee_str)
    print("{} bestellt".format(coffee_str))


def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc, properties):
        if rc == 0:
            print("Connected to MQTT Broker!")
            client.subscribe([(topic_rot, 0), (topic_blau,0), (topic_weiss,0)])
        else:
            print("Failed to connect, return code %d\n", rc)
    
    #Control over buttons
    def on_message(client, userdata, msg):
        global index
            
        current_msg = msg.payload.decode()
        current_topic = msg.topic
        
        #Liste mit Kaffeesorten
        #list = ['kleinerKaffee', 'grosserKaffee', 'cappuccino', 'latteMacchiato', 'milchschaum', 'heissesWasser'];        
        
        if current_topic == "button_rot":
            if current_msg == "1":
                index = index +1   
            if index > 5:
                index = 0 #Reset position in app and list of coffee specialities
            if index == 0:
                wahl_kk()             
            elif index == 1:
                wahl_gk()           
            elif index == 2:
                wahl_cap()
            elif index == 3:
                wahl_lm()
            elif index == 4:
                wahl_ms()
            elif index == 5:
                wahl_hw()
            else:
                print("Fehler!")
            
        #Button Blau - Is used to confirm the selected coffee. It publishes the msg to the topic
        if current_topic == "button_blau":
            #Kleiner Kaffee
            if index == 0:
                reset_menu()
                publish_msg(client, "Kleiner Kaffee")
            
            #Grosser Kaffee
            elif index == 1:
                reset_menu()
                publish_msg(client, "Grosser Kaffee")
               
            #Cappucino
            elif index == 2:
                reset_menu()
                publish_msg(client, "Cappuccino")
                
            #Latte Macchiato
            elif index == 3:
                reset_menu()
                publish_msg(client, "Latte Macchiato")
                
            #Milchschaum
            elif index == 4:
                reset_menu()
                publish_msg(client, "Milchschaum")
                
            #Heisses Wasser
            elif index == 5:
                reset_menu()
                publish_msg(client, "Heisses Wasser")
                
            else:
                print("Fehler Button blau")
                
                
        #Button Wei§ - Resets the system.
        if current_topic == "button_weiss":
            index = 0
            reset_menu()
            print("Button Weiss")            

    client = mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker, port)
    
    thread = threading.Thread(target=client.loop_forever)
    thread.start()
    
    return client

# Control over display directly
def selection(client: mqtt_client):
    
    global kaffee_klein_btn
    global kaffee_gross_btn
    global cappuccino_btn
    global latte_btn
    global milchschaum_btn
    global wasser_btn
    
    # Settings for fonts
    normal_font = ('Arial', 8, 'normal')
    bold_font = ('Arial', 8, 'bold')
    global wahl_kk
    
    
    def display_button_pressed(val):
        if val ==1:
            wahl_kk()
        elif val==2:
            wahl_gk()
        elif val==3:
            wahl_cap()
        elif val==4:
            wahl_lm()
        elif val==5:
            wahl_ms()
        elif val==6:
            wahl_hw()
        
    #Command Auswahl bestÃ¤tigen
    def confirm_cmd():
        global choice
        
        if (choice=="Small Coffee"):
            reset_menu()
            publish_msg(client, "Kleiner Kaffee")
            
        elif (choice=="Big Coffee"):
            reset_menu()
            publish_msg(client, "Grosser Kaffee")
            
        elif (choice=="Cappucino"):
            reset_menu()
            publish_msg(client, "Cappucino")
            
        elif (choice=="Latte Macchiato"):
            reset_menu()
            publish_msg(client, "Latte Macchiato")
            
        elif (choice=="Milk Foam"):
            reset_menu()
            publish_msg(client, "Milchschaum")
            
        elif (choice=="Hot Water"):
            reset_menu()
            publish_msg(client, "Heisses Wasser")
            
        
    #Command Vorgang abbrechen
    def abbr_cmd():
            reset_menu()

############################## LAYOUT of Display Interface #########################################  
    # Layout Frontend
    front = Tk()
    front.title('Barrierefreie Kaffeemaschine')    # Titel Frontend
    front.configure(bg='navy')                     # Hintergrundfarbe
    front.geometry("760x370")                      # AbmaÃŸe Frontend
    #front.iconbitmap("HSO_Logo.png")    
    
    # Willkommen-Text
    front1_label = Label(front, text="Willkommen bei der Lebenshilfe Kinzig- und Elztal!", anchor=(NW))
    front1_label.grid(row=0, column=0, columnspan=2, pady=(30, 0), padx=(30,10), sticky='nsew')
    front1_label.configure(bg='navy', fg='white', font=("Arial", 16))
    
    # Untertitel
    front2_label = Label(front, text="Bestellen Sie nachfolgend den Kaffee Ihrer Wahl:", anchor=(NW))
    front2_label.grid(row=1, column=0, columnspan=2, pady=(10, 20), padx=(30,10), sticky='nsew')
    front2_label.configure(bg='navy', fg='white', font=("Arial", 11))
    
    # "Entwickelt von" Text
    front3_label = Label(front, text="Entwickelt von")
    front3_label.grid(row=0, column=2, pady=(15, 0), padx=(30,0), sticky='nsew')
    front3_label.configure(bg='navy', fg='white', font=("Arial", 8))
    
    
    ############################### LOGOS #########################################
    # HSO-Logo einfÃ¼gen
    logo = Image.open("HSO_Logo_weiss.png")                 # Logo Ã¶ffnen (MaÃŸe: 1576x661)
    resize_logo = logo.resize((127, 55), Image.LANCZOS) # Das Bild mit der resize()-Methode verkleinern
    lg = ImageTk.PhotoImage(resize_logo)                    # Verwende ImageTk, um das Bild fÃ¼r Tkinter vorzubereiten
    label_logo = Label(front, image=lg)                     # Erstelle ein Label und fÃ¼ge das Bild hinzu
    label_logo.configure(bg='navy')                         # Hintergrundfarbe vom Label Ã¤ndern
    label_logo.grid(column=2, row=1, padx=(47,0))           # Platz des Labels im Raster
    #label_logo.pack()                                      # Packe das Label in das Frame
    
    
    ############################### BUTTONS #######################################
    # Konstante GrÃ¶ÃŸen fÃ¼r die Buttons festlegen
    button_width = 30  # Breite der Buttons
    button_height = 2  # HÃ¶he der Buttons
    
    global kaffee_klein_btn
    global kaffee_gross_btn
    global cappuccino_btn
    global latte_btn
    global milchschaum_btn
    global wasser_btn
    
    # Buttons fÃ¼r GetrÃ¤nke
    kaffee_klein_btn = Button(front, bg= "white", text="Kleiner Kaffee", command=lambda *args: display_button_pressed(1), width=button_width, height=button_height)
    kaffee_klein_btn.grid(row=2, column=0, pady=20, padx=(30,0), sticky='nsew')
    kaffee_klein_btn.configure(font=normal_font)
    
    kaffee_gross_btn = Button(front, bg= "white", text="Grosser Kaffee", command=lambda *args: display_button_pressed(2), width=button_width, height=button_height)
    kaffee_gross_btn.grid(row=2, column=1, pady=20, padx=(30,0), sticky='nsew')
    kaffee_gross_btn.configure(font=normal_font)
    
    cappuccino_btn = Button(front, bg= "white", text="Cappuccino", command=lambda *args: display_button_pressed(3), width=button_width, height=button_height)
    cappuccino_btn.grid(row=3, column=0, pady=20, padx=(30,0), sticky='nsew')
    cappuccino_btn.configure(font=normal_font)
    
    latte_btn = Button(front, bg= "white", text="Latte Macchiato", command=lambda *args: display_button_pressed(4), width=button_width, height=button_height)
    latte_btn.grid(row=3, column=1, pady=20, padx=(30,0), sticky='nsew')
    latte_btn.configure(font=normal_font)
    
    milchschaum_btn = Button(front, bg= "white", text="Milchschaum", command=lambda *args: display_button_pressed(5), width=button_width, height=button_height)
    milchschaum_btn.grid(row=4, column=0, pady=20, padx=(30,0), sticky='nsew')
    milchschaum_btn.configure(font=normal_font)
    
    wasser_btn = Button(front, bg= "white", text="Heisses Wasser", command=lambda *args: display_button_pressed(6), width=button_width, height=button_height)
    wasser_btn.grid(row=4, column=1, pady=20, padx=(30,0), sticky='nsew')
    wasser_btn.configure(font=normal_font)
    
    # Buttons fÃ¼r AuswahlbestÃ¤tigung und Abbrechen
    bestaetigen_btn = Button(front, bg= "white", text="Auswahl bestŠtigen", command=confirm_cmd, width=button_width, height=button_height)
    bestaetigen_btn.grid(row=3, column=2, pady=20, padx=(40,0), sticky='nsew')
    bestaetigen_btn.configure(font=normal_font)
    
    abbrechen_btn = Button(front, bg= "white", text="Vorgang abbrechen", command=abbr_cmd, width=button_width, height=button_height)
    abbrechen_btn.grid(row=4, column=2, pady=20, padx=(40,0), sticky='nsew')
    abbrechen_btn.configure(font=normal_font)
    
    
    ########################### CONFIGURATION GRIDS ###############################
    # Create list of buttons
    #button_list = [front1_label, front2_label, front3_label, label_logo, kaffee_klein_btn, kaffee_gross_btn, cappuccino_btn, latte_btn, milchschaum_btn, wasser_btn, bestaetigen_btn, abbrechen_btn]
    button_list = [label_logo, kaffee_klein_btn, kaffee_gross_btn, cappuccino_btn, latte_btn, milchschaum_btn, wasser_btn, bestaetigen_btn, abbrechen_btn]
    
    # Define row and column number
    row_number = 0
    column_number = 0
    
    # Loop through the list and configure each row/column automatically
    for button in button_list:
        Grid.rowconfigure(front, row_number, weight=1)
        Grid.columnconfigure(front, column_number, weight=1)
        row_number += 1
        column_number += 1

    def resize(event):
        # Width of the app
        size = event.width/55  #Size fÃ¼r Button-Text
        size1 = event.width/30 #Size fÃ¼r Label-Text (Willkommen)
        size2 = event.width/40 #Size fÃ¼r Label-Text (Untertitel)
        size3 = event.width/60 #Size fÃ¼r Label-Text (Entwickelt von)
        size4 = event.width/55 #Size fÃ¼r Logo-Label
        
        #Change button text size
        kaffee_gross_btn.configure(font=('Arial', int(size)))
        kaffee_klein_btn.configure(font=('Arial', int(size)))
        cappuccino_btn.configure(font=('Arial', int(size)))
        latte_btn.configure(font=('Arial', int(size)))
        milchschaum_btn.configure(font=('Arial', int(size)))
        wasser_btn.configure(font=('Arial', int(size)))
        bestaetigen_btn.configure(font=('Arial', int(size)))
        abbrechen_btn.configure(font=('Arial', int(size)))
        
        #Change Label text sizes
        front1_label.configure(font=('Arial', int(size1)))
        front2_label.configure(font=('Arial', int(size2)))
        front3_label.configure(font=('Arial', int(size3)))
    
    front.bind('<Configure>', resize)
    front.mainloop()


def run():
    client = connect_mqtt()  
    selection(client)


if __name__ == '__main__':
    run()
    

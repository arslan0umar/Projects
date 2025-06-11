import serial, threading, time, os, json, datetime
from tkinter import *
from tkinter import ttk, simpledialog, filedialog, messagebox
from PIL import Image, ImageTk, ImageDraw
import csv
import shutil

# — Configuration —
PORT = 'COM5'
BAUD = 9600
IMG_SIZE = 320
RESET_TIME = 3000  # in milliseconds
ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "admin"
USERS_FILE = "users.json"
LOGS_FILE = "access_logs.csv"
IMAGES_DIR = "user_images"
DENIED_IMAGE_PATH = "not_allowed.png"  # Transparent PNG image
card_var = None
status_var = None
scan_btn = None

# Ensure directories exist
if not os.path.exists(IMAGES_DIR):
    os.makedirs(IMAGES_DIR)

# Load users from file
def load_users():
    if os.path.exists(USERS_FILE):
        try:
            with open(USERS_FILE, "r") as f:
                return json.load(f)
        except Exception as e:
            print(f"Error loading users file: {e}")
    return {}

# Save users to file
def save_users(users_data):
    with open(USERS_FILE, "w") as f:
        json.dump(users_data, f, indent=4)

# Load or initialize users
USERS = load_users()

# Log access attempts
def log_access(uid, name, status, timestamp=None):
    if timestamp is None:
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # Create log file if it doesn't exist
    file_exists = os.path.exists(LOGS_FILE)
    
    with open(LOGS_FILE, "a", newline='') as f:
        writer = csv.writer(f)
        if not file_exists:
            writer.writerow(["Timestamp", "Card ID", "Name", "Status"])
        writer.writerow([timestamp, uid, name, status])

# — Setup Window —
root = Tk()
root.title("Smart RFID Access Control")
root.geometry("1000x700")
root.resizable(True, True)

# — Colors —
colors = {
    "bg": "#f5f5f7",
    "white": "#ffffff",
    "primary": "#0071e3",
    "success": "#34c759",
    "error": "#ff3b30",
    "text": "#1d1d1f",
    "secondary": "#6e6e73"
}

# Configure the root window
root.configure(bg=colors["bg"])

# Custom class for rounded corner frame
class RoundedFrame(Canvas):
    def __init__(self, parent, width, height, radius, bg, border_color, border_width=1, **kwargs):
        super().__init__(parent, width=width, height=height, bg=bg, highlightthickness=0, **kwargs)
        self.radius = radius
        
        # Create rounded rectangle
        self.rounded_rect = self.create_rounded_rectangle(
            border_width, border_width, 
            width - border_width, height - border_width, 
            radius=radius, fill=bg, outline=border_color, width=border_width
        )
        
    def create_rounded_rectangle(self, x1, y1, x2, y2, radius, **kwargs):
        # Create points for a rounded rectangle
        points = [
            x1 + radius, y1,
            x2 - radius, y1,
            x2, y1,
            x2, y1 + radius,
            x2, y2 - radius,
            x2, y2,
            x2 - radius, y2,
            x1 + radius, y2,
            x1, y2,
            x1, y2 - radius,
            x1, y1 + radius,
            x1, y1
        ]
        return self.create_polygon(points, smooth=True, **kwargs)

# — Create a modern frame with truly rounded corners —
main_width, main_height = 600, 650  # Increased height to fit name label
frame_radius = 15

# Create the rounded frame container
frame_container = RoundedFrame(
    root, 
    width=main_width, 
    height=main_height, 
    radius=frame_radius, 
    bg=colors["white"], 
    border_color=colors["secondary"], 
    border_width=4
)
frame_container.place(relx=0.5, rely=0.5, anchor=CENTER)

# Create inner content frame
content_container = Frame(frame_container, bg=colors["white"])
content_container.place(x=0, y=0, width=main_width, height=main_height)

# — Header with gradient —
header_frame = Frame(content_container, bg=colors["primary"], height=80)
header_frame.pack(fill=X)

header_label = Label(header_frame, text="Smart Access Control", 
                    font=("Helvetica", 24, "bold"), fg=colors["white"], bg=colors["primary"])
header_label.pack(pady=20)

# — Content area —
content_frame = Frame(content_container, bg=colors["white"])
content_frame.pack(fill=BOTH, expand=True, padx=40, pady=30)

# Create a frame for the initial options
options_frame = Frame(content_frame, bg=colors["white"])
options_frame.pack(fill=BOTH, expand=True)

# Create a frame for the main RFID scanning functionality
scanning_frame = Frame(content_frame, bg=colors["white"])

# Status with modern font
status_lbl = Label(scanning_frame, text="Ready to Scan", 
                  font=("Helvetica", 36, "bold"), 
                  fg=colors["text"], bg=colors["white"])
status_lbl.pack(pady=(0, 20))

# Canvas for the image with proper background color
image_canvas = Canvas(scanning_frame, width=IMG_SIZE + 40, height=IMG_SIZE + 40,
                    bg=colors["white"], highlightthickness=0)
image_canvas.pack()

# Clear space for name label
spacer = Frame(scanning_frame, height=10, bg=colors["white"])
spacer.pack()

# Name display with nicer font
name_frame = Frame(scanning_frame, bg=colors["white"])
name_frame.pack(fill=X, pady=10)

name_lbl = Label(name_frame, text="", 
                font=("Helvetica", 28, "bold"), 
                fg=colors["primary"], bg=colors["white"])
name_lbl.pack(fill=X)

# Add a footer with additional info
footer_lbl = Label(scanning_frame, text="Please place your card on the reader", 
                  font=("Helvetica", 14), 
                  fg=colors["secondary"], bg=colors["white"])
footer_lbl.pack(pady=(10, 20))

# Add back button for scanning frame
back_btn_frame = Frame(scanning_frame, bg=colors["white"])
back_btn_frame.pack(fill=X, pady=10)

back_btn = Button(back_btn_frame, text="Back to Main Menu", 
                 font=("Helvetica", 12),
                 bg=colors["primary"], fg=colors["white"],
                 command=lambda: [stop_animation(), show_main_options()])
back_btn.pack(pady=5)

# — Helper Functions —
def create_circular_image(img_path):
    """Create a circular image with proper transparency handling"""
    try:
        # Load image and convert to RGBA to ensure transparency support
        img = Image.open(img_path).convert("RGBA")
    except Exception as e:
        print(f"Error loading image: {e}")
        # Create fallback image if file not found
        img = Image.new("RGBA", (IMG_SIZE, IMG_SIZE), (200, 200, 200, 255))
    
    # Resize image
    img = img.resize((IMG_SIZE, IMG_SIZE))
    
    # Create circular mask
    mask = Image.new("L", (IMG_SIZE, IMG_SIZE), 0)
    draw = ImageDraw.Draw(mask)
    draw.ellipse((0, 0, IMG_SIZE, IMG_SIZE), fill=255)
    
    # Create new transparent image
    result = Image.new("RGBA", (IMG_SIZE, IMG_SIZE), (0, 0, 0, 0))
    
    # Paste original image using mask
    result.paste(img, (0, 0), mask)
    
    return result

def fade_in_text(label, text, color, i=0):
    """Animate text appearance"""
    if i <= len(text):
        label.config(text=text[:i], fg=color)
        root.after(40, fade_in_text, label, text, color, i + 1)

def pulse_effect(canvas, item, size=0, increasing=True, max_size=10):
    """Create a pulsing animation effect for the waiting circle"""
    if not hasattr(pulse_effect, "active") or not pulse_effect.active:
        return
        
    if increasing:
        size += 0.5
        if size >= max_size:
            increasing = False
    else:
        size -= 0.5
        if size <= 0:
            increasing = True
            
    # Update the oval size
    canvas.coords(item, 
                20-size, 20-size, 
                IMG_SIZE+20+size, IMG_SIZE+20+size)
    
    # Continue animation
    root.after(50, pulse_effect, canvas, item, size, increasing)

def start_waiting_animation():
    """Create the waiting for card animation"""
    image_canvas.delete("all")
    
    # Draw waiting circle with subtle gray fill
    waiting_circle = image_canvas.create_oval(20, 20, IMG_SIZE+20, IMG_SIZE+20, 
                                           outline=colors["primary"], width=3,
                                           fill="#f8f8fa")
    
    # Add RFID icon or text in the middle
    image_canvas.create_text(IMG_SIZE//2 + 20, IMG_SIZE//2 + 20,
                          text="RFID", font=("Helvetica", 36, "bold"),
                          fill=colors["secondary"])
    
    # Start pulsing animation
    pulse_effect.active = True
    pulse_effect(image_canvas, waiting_circle)
    
    # Update labels
    fade_in_text(status_lbl, "Ready to Scan", colors["text"])
    name_lbl.config(text="")  # Clear the name label
    footer_lbl.config(text="Please place your card on the reader")

def stop_animation():
    """Stop any ongoing animations"""
    pulse_effect.active = False

def show_idle_screen():
    """Reset UI to waiting state"""
    global last_read_uid
    name_lbl.config(text="")
    stop_animation()
    start_waiting_animation()
    # Reset the last read UID so cards can be scanned again
    last_read_uid = None

def show_user(uid):
    """Display user access result"""
    stop_animation()
    uid = uid.strip().upper()
    user = USERS.get(uid)
    image_canvas.delete("all")
    
    # Log this access attempt
    status = "ACCESS GRANTED" if user else "ACCESS DENIED"
    user_name = user["name"] if user else "Unknown User"
    log_access(uid, user_name, status)
    
    if user:
        # === ACCESS GRANTED ===
        try:
            # Create circular image
            circular_img = create_circular_image(user["image"])
            photo = ImageTk.PhotoImage(circular_img)
            image_canvas.user_image = photo  # Keep reference to prevent garbage collection
            
            # Draw a success border (green circle)
            image_canvas.create_oval(10, 10, IMG_SIZE+30, IMG_SIZE+30, 
                                  outline=colors["success"], width=5)
            
            # Draw image
            image_canvas.create_image(IMG_SIZE//2 + 20, IMG_SIZE//2 + 20, 
                                   image=photo)
            
            # Create success checkmark icon with animation
            check_x, check_y = IMG_SIZE-40, 40
            image_canvas.create_oval(check_x-15, check_y-15, check_x+15, check_y+15,
                                  fill=colors["success"], outline="")
            image_canvas.create_text(check_x, check_y, text="✓", 
                                  fill="white", font=("Helvetica", 20, "bold"))
            
            # Update text with animation
            fade_in_text(status_lbl, "✅ Allowed", colors["success"])
            
            # Set name directly with no animation
            name = user["name"]
            print(f"Setting name to: {name}")  # Debug print
            name_lbl.config(text=name)
            
            # Make name visible and update footer
            footer_lbl.config(text="Welcome! You may proceed.")
            
        except Exception as e:
            print(f"Error displaying user image: {e}")
            name_lbl.config(text=f"Error: {e}")
            
    else:
        # === ACCESS DENIED ===
        try:
            # Create circular image with border for denied access
            circular_img = create_circular_image(DENIED_IMAGE_PATH)
            photo = ImageTk.PhotoImage(circular_img)
            image_canvas.denied_image = photo  # Keep reference
            
            # Draw error border
            image_canvas.create_oval(10, 10, IMG_SIZE+30, IMG_SIZE+30, 
                                  outline=colors["error"], width=5)
            
            # Draw image
            image_canvas.create_image(IMG_SIZE//2 + 20, IMG_SIZE//2 + 20, 
                                   image=photo)
            
            # Create error X icon
            x_pos_x, x_pos_y = IMG_SIZE-40, 40
            image_canvas.create_oval(x_pos_x-15, x_pos_y-15, x_pos_x+15, x_pos_y+15,
                                  fill=colors["error"], outline="")
            image_canvas.create_text(x_pos_x, x_pos_y, text="✕", 
                                  fill="white", font=("Helvetica", 16, "bold"))
            
            # Update text
            fade_in_text(status_lbl, "❌ Not Allowed", colors["error"])
            name_lbl.config(text="Unknown User")
            footer_lbl.config(text="Sorry, you don't have permission.")
            
        except Exception as e:
            print(f"Error displaying denied image: {e}")
            name_lbl.config(text=f"Error: {e}")
    
    # Reset after timeout
    root.after(RESET_TIME, show_idle_screen)

def read_loop():
    """RFID reader thread function"""
    global last_read_uid
    last_read_uid = None
    
    while True:
        try:
            if ser.in_waiting:
                uid = ser.readline().decode().strip().upper()
                if uid and uid != last_read_uid:
                    last_read_uid = uid
                    print(f"Card read: {uid}")
                    
                    # Check if the card is in the USERS dictionary
                    is_authorized = uid in USERS
                    
                    # Send response back to Arduino
                    if is_authorized:
                        ser.write(b'1')  # Authorized
                    else:
                        ser.write(b'0')  # Not authorized
                    
                    # Process the card in the UI
                    root.after(0, process_card_read, uid)
        except Exception as e:
            print(f"Serial read error: {e}")
        time.sleep(0.1)

def process_card_read(uid):
    """Process a card read based on current application state"""
    global current_state
    
    if current_state == "scanning":
        show_user(uid)
    elif current_state == "registering":
        register_new_user(uid)
    else:
        print(f"Ignoring card read in state: {current_state}")

# — Main Options UI —
def show_main_options():
    """Show the main options menu"""
    global current_state
    current_state = "main_menu"
    
    # Hide scanning frame and show options frame
    scanning_frame.pack_forget()
    options_frame.pack(fill=BOTH, expand=True)
    
    # Hide the global back button if it exists
    if 'main_back_btn_frame' in globals() and main_back_btn_frame.winfo_exists():
        main_back_btn_frame.pack_forget()
    
    # Clear the options frame
    for widget in options_frame.winfo_children():
        widget.destroy()
    
    # Container with proper padding
    main_content = Frame(options_frame, bg=colors["white"], padx=30, pady=20)
    main_content.pack(fill=BOTH, expand=True)
    
    # Title with wraplength to prevent text cutting
    title_lbl = Label(main_content, text="Welcome to Smart Access Control", 
                    font=("Helvetica", 24, "bold"), 
                    fg=colors["primary"], bg=colors["white"],
                    wraplength=500)  # Set wraplength to prevent text cutting
    title_lbl.pack(pady=20)
    
    # Description
    desc_lbl = Label(main_content, text="Please select an option below", 
                   font=("Helvetica", 14), 
                   fg=colors["text"], bg=colors["white"])
    desc_lbl.pack(pady=10)
    
    # Buttons frame
    buttons_frame = Frame(main_content, bg=colors["white"])
    buttons_frame.pack(pady=40)
    
    # Style buttons
    button_style = {
        "font": ("Helvetica", 16),
        "bg": colors["primary"],
        "fg": colors["white"],
        "width": 20,
        "height": 2,
        "borderwidth": 0,
        "relief": "flat"
    }
    
    # Scan RFID button
    scan_btn = Button(buttons_frame, text="Scan RFID Card", 
                     command=show_scanning_ui, **button_style)
    scan_btn.pack(pady=10)
    
    # Admin Login button
    admin_btn = Button(buttons_frame, text="Admin Login", 
                      command=admin_login, **button_style)
    admin_btn.pack(pady=10)

def show_scanning_ui():
    """Show the RFID scanning interface"""
    global current_state
    current_state = "scanning"
    
    # Hide options frame and show scanning frame
    options_frame.pack_forget()
    scanning_frame.pack(fill=BOTH, expand=True)
    
    # Create a frame for back button at the bottom of the main window (not inside scanning_frame)
    global main_back_btn_frame
    if 'main_back_btn_frame' in globals() and main_back_btn_frame.winfo_exists():
        main_back_btn_frame.pack(side=BOTTOM, fill=X, padx=20, pady=10)
    else:
        main_back_btn_frame = Frame(root, bg=colors["bg"], padx=20, pady=10)
        main_back_btn_frame.pack(side=BOTTOM, fill=X)
        
        main_back_btn = Button(main_back_btn_frame, text="Return to Main Menu", 
                            font=("Helvetica", 14, "bold"),
                            bg=colors["primary"], fg=colors["white"],
                            padx=20, pady=10,
                            command=lambda: [stop_animation(), main_back_btn_frame.pack_forget(), show_main_options()])
        main_back_btn.pack(fill=X)
    
    # Initialize scanning screen
    show_idle_screen()

# — Admin Functions —
def admin_login():
    """Handle admin login"""
    # Create login dialog
    login_window = Toplevel(root)
    login_window.title("Admin Login")
    login_window.geometry("450x350")  # Increased window size
    login_window.configure(bg=colors["bg"])
    login_window.resizable(False, False)
    
    # Center the window
    login_window.transient(root)
    login_window.grab_set()
    
    # Simple frame for content
    login_frame = Frame(login_window, bg=colors["white"], padx=30, pady=30)
    login_frame.pack(fill=BOTH, expand=True, padx=20, pady=20)
    
    # Login form
    title_lbl = Label(login_frame, text="Admin Login", 
                    font=("Helvetica", 18, "bold"), 
                    fg=colors["primary"], bg=colors["white"])
    title_lbl.pack(pady=(0, 20))
    
    # Username
    username_frame = Frame(login_frame, bg=colors["white"])
    username_frame.pack(fill=X, pady=5)
    
    username_lbl = Label(username_frame, text="Username:", 
                       font=("Helvetica", 12), 
                       fg=colors["text"], bg=colors["white"], width=10, anchor="w")
    username_lbl.pack(side=LEFT, padx=5)
    
    username_entry = Entry(username_frame, font=("Helvetica", 12), width=20)
    username_entry.pack(side=LEFT, padx=5)
    username_entry.focus_set()
    
    # Password
    password_frame = Frame(login_frame, bg=colors["white"])
    password_frame.pack(fill=X, pady=5)
    
    password_lbl = Label(password_frame, text="Password:", 
                       font=("Helvetica", 12), 
                       fg=colors["text"], bg=colors["white"], width=10, anchor="w")
    password_lbl.pack(side=LEFT, padx=5)
    
    password_entry = Entry(password_frame, font=("Helvetica", 12), width=20, show="*")
    password_entry.pack(side=LEFT, padx=5)
    
    # Message
    message_lbl = Label(login_frame, text="", 
                      font=("Helvetica", 12), 
                      fg=colors["error"], bg=colors["white"])
    message_lbl.pack(pady=10)
    
    # Button
    def validate_login():
        username = username_entry.get()
        password = password_entry.get()
        
        if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
            login_window.destroy()
            admin_panel()
        else:
            message_lbl.config(text="Invalid username or password")
    
    # Button container frame
    button_frame = Frame(login_frame, bg=colors["white"])
    button_frame.pack(pady=10)
    
    # Login button - increased size
    login_btn = Button(button_frame, text="Login", 
                     font=("Helvetica", 12),
                     bg=colors["primary"], fg=colors["white"],
                     width=12, height=1, padx=10, pady=5,
                     command=validate_login)
    login_btn.pack(side=LEFT, padx=10)
    
    # Cancel button - increased size
    cancel_btn = Button(button_frame, text="Cancel", 
                      font=("Helvetica", 12),
                      bg=colors["secondary"], fg=colors["white"],
                      width=12, height=1, padx=10, pady=5,
                      command=login_window.destroy)
    cancel_btn.pack(side=LEFT, padx=10)
    
    # Bind Enter key to validate login
    login_window.bind("<Return>", lambda event: validate_login())

def admin_panel():
    """Display admin panel"""
    global current_state
    current_state = "admin"
    
    # Hide main frames
    options_frame.pack_forget()
    scanning_frame.pack_forget()
    
    # Create admin panel frame
    admin_frame = Frame(content_frame, bg=colors["white"])
    admin_frame.pack(fill=BOTH, expand=True)
    
    # Admin panel title
    title_lbl = Label(admin_frame, text="Admin Panel", 
                     font=("Helvetica", 24, "bold"), 
                     fg=colors["primary"], bg=colors["white"])
    title_lbl.pack(pady=10)
    
    # Create notebook for tabs
    notebook = ttk.Notebook(admin_frame)
    notebook.pack(fill=BOTH, expand=True, padx=20, pady=10)
    
    # Register User tab
    register_tab = Frame(notebook, bg=colors["white"])
    notebook.add(register_tab, text="Register User")
    
    # View Logs tab
    logs_tab = Frame(notebook, bg=colors["white"])
    notebook.add(logs_tab, text="Access Logs")
    
    # User Management tab
    users_tab = Frame(notebook, bg=colors["white"])
    notebook.add(users_tab, text="User Management")
    
    # Setup Register User tab
    setup_register_tab(register_tab)
    
    # Setup Logs tab
    setup_logs_tab(logs_tab)
    
    # Setup Users Management tab
    setup_users_tab(users_tab)
    
    # Create a global back button at the bottom of the main window
    global main_back_btn_frame
    if 'main_back_btn_frame' in globals() and main_back_btn_frame.winfo_exists():
        main_back_btn_frame.pack_forget()
    
    main_back_btn_frame = Frame(root, bg=colors["bg"], padx=20, pady=10)
    main_back_btn_frame.pack(side=BOTTOM, fill=X)
    
    main_back_btn = Button(main_back_btn_frame, text="Return to Main Menu", 
                        font=("Helvetica", 14, "bold"),
                        bg=colors["primary"], fg=colors["white"],
                        padx=20, pady=10,
                        command=lambda: [admin_frame.destroy(), main_back_btn_frame.pack_forget(), show_main_options()])
    main_back_btn.pack(fill=X)

def setup_register_tab(parent):
    """Set up the register user tab"""
    global card_var, status_var, scan_btn
    
    # Create form with proper padding
    form_frame = Frame(parent, bg=colors["white"], padx=30, pady=15)
    form_frame.pack(fill=BOTH, expand=True)
    
    # Instructions with proper wraplength to prevent text cutting
    instr_lbl = Label(form_frame, 
                    text="Register new user by scanning their RFID card and entering details", 
                    font=("Helvetica", 12), 
                    fg=colors["text"], bg=colors["white"], 
                    wraplength=450,  # Adjusted to prevent text cutting
                    justify=LEFT)
    instr_lbl.pack(pady=10, anchor=W)
    
    # Card ID frame
    card_frame = Frame(form_frame, bg=colors["white"])
    card_frame.pack(fill=X, pady=10)
    
    card_lbl = Label(card_frame, text="Card ID:", 
                   font=("Helvetica", 12), 
                   fg=colors["text"], bg=colors["white"], width=10, anchor="w")
    card_lbl.pack(side=LEFT, padx=5)
    
    card_var = StringVar()  # Make sure this is a global variable
    card_entry = Entry(card_frame, font=("Helvetica", 12), width=20, textvariable=card_var, state="readonly")
    card_entry.pack(side=LEFT, padx=5)
    
    # Scan button with adjusted size and padding
    def start_card_scan():
        global current_state
        current_state = "registering"
        scan_btn.config(text="Waiting for card...", state="disabled")
        status_var.set("Please scan the RFID card now")
    
    scan_btn = Button(card_frame, text="Scan Card", 
                    font=("Helvetica", 10),
                    bg=colors["primary"], fg=colors["white"],
                    padx=5, pady=2,  # Add padding for better text fit
                    command=start_card_scan)
    scan_btn.pack(side=LEFT, padx=5)
    
    # Name frame
    name_frame = Frame(form_frame, bg=colors["white"])
    name_frame.pack(fill=X, pady=10)
    
    name_lbl = Label(name_frame, text="Name:", 
                   font=("Helvetica", 12), 
                   fg=colors["text"], bg=colors["white"], width=10, anchor="w")
    name_lbl.pack(side=LEFT, padx=5)
    
    name_var = StringVar()
    name_entry = Entry(name_frame, font=("Helvetica", 12), width=30, textvariable=name_var)
    name_entry.pack(side=LEFT, padx=5)
    
    # Image frame
    image_frame = Frame(form_frame, bg=colors["white"])
    image_frame.pack(fill=X, pady=10)
    
    image_lbl = Label(image_frame, text="Image:", 
                    font=("Helvetica", 12), 
                    fg=colors["text"], bg=colors["white"], width=10, anchor="w")
    image_lbl.pack(side=LEFT, padx=5)
    
    image_path_var = StringVar()
    image_entry = Entry(image_frame, font=("Helvetica", 12), width=25, textvariable=image_path_var, state="readonly")
    image_entry.pack(side=LEFT, padx=5)
    
    # Create preview frame for register tab
    preview_frame = Frame(form_frame, bg=colors["white"])
    preview_frame.pack(fill=X, pady=10)
    
    # Function to display preview of the image
    def display_preview(img_path):
        for widget in preview_frame.winfo_children():
            widget.destroy()
            
        try:
            # Load and resize image for preview
            img = Image.open(img_path)
            img = img.resize((100, 100), Image.LANCZOS)
            photo = ImageTk.PhotoImage(img)
            
            # Display image
            img_label = Label(preview_frame, image=photo, bg=colors["white"])
            img_label.image = photo  # Keep reference
            img_label.pack(pady=10)
        except Exception as e:
            error_label = Label(preview_frame, text=f"Cannot preview image: {e}", 
                              fg=colors["error"], bg=colors["white"])
            error_label.pack(pady=10)
    
    # Browse button with adjusted width and padding
    def browse_image():
        file_path = filedialog.askopenfilename(
            title="Select Image",
            filetypes=[("Image files", "*.jpg *.jpeg *.png")]
        )
        if file_path:
            image_path_var.set(file_path)
    
    browse_btn = Button(image_frame, text="Browse", 
                      font=("Helvetica", 10),
                      bg=colors["secondary"], fg=colors["white"],
                      width=8, padx=5, pady=2,  # Adjusted width and padding
                      command=browse_image)
    browse_btn.pack(side=LEFT, padx=5)

    # Status message
    status_var = StringVar()  # Make sure this is a global variable
    status_lbl = Label(form_frame, textvariable=status_var, 
                     font=("Helvetica", 12), 
                     fg=colors["secondary"], bg=colors["white"])
    status_lbl.pack(pady=10)
    
    # Register button
    def save_user():
        card_id = card_var.get().strip().upper()
        name = name_var.get().strip()
        image_path = image_path_var.get().strip()
        
        if not card_id:
            status_var.set("Please scan an RFID card first")
            return
        
        if not name:
            status_var.set("Please enter a name")
            return
        
        if not image_path:
            status_var.set("Please select an image")
            return
        
        # Copy image to user_images directory with a unique name
        try:
            extension = os.path.splitext(image_path)[1]
            new_image_name = f"{card_id}{extension}"
            new_image_path = os.path.join(IMAGES_DIR, new_image_name)
            
            shutil.copy2(image_path, new_image_path)
            
            # Add user to database
            USERS[card_id] = {
                "name": name,
                "image": new_image_path
            }
            
            # Save to file
            save_users(USERS)
            
            # Reset form
            card_var.set("")
            name_var.set("")
            image_path_var.set("")
            status_var.set(f"User {name} successfully registered!")
            scan_btn.config(text="Scan Card", state="normal")
            current_state = "admin"
            
            # Clear the preview
            for widget in preview_frame.winfo_children():
                widget.destroy()
                
        except Exception as e:
            status_var.set(f"Error registering user: {e}")
    
    register_btn = Button(form_frame, text="Register User", 
                        font=("Helvetica", 12),
                        bg=colors["success"], fg=colors["white"],
                        width=15, command=save_user)
    register_btn.pack(pady=10)

def register_new_user(uid):
    """Register a new user with the given UID"""
    global current_state, card_var, scan_btn, status_var
    
    # Directly update the card_var StringVar
    if card_var is not None:
        card_var.set(uid)
    
    # Update the scan button
    if scan_btn is not None:
        scan_btn.config(text="Scan Card", state="normal")
    
    # Update status message
    if status_var is not None:
        status_var.set(f"Card ID {uid} scanned successfully!")
    else:
        print("status_var is None, can't update status message")
    
    # Switch state back to admin
    current_state = "admin"

def setup_logs_tab(parent):
    """Set up the logs tab"""
    # Create frame for logs - using pack instead of grid to avoid conflicts
    logs_frame = Frame(parent, bg=colors["white"], padx=20, pady=10)
    logs_frame.pack(fill=BOTH, expand=True)
    
    # Title
    title_lbl = Label(logs_frame, text="Access Logs", 
                    font=("Helvetica", 14, "bold"), 
                    fg=colors["primary"], bg=colors["white"])
    title_lbl.pack(pady=10)
    
    # Create a container frame for the treeview and scrollbars
    tree_frame = Frame(logs_frame, bg=colors["white"])
    tree_frame.pack(fill=BOTH, expand=True, pady=10)
    
    # Treeview for logs
    columns = ("timestamp", "card_id", "name", "status")
    tree = ttk.Treeview(tree_frame, columns=columns, show="headings")
    
    # Configure column widths
    tree.column("timestamp", width=150, anchor="center")
    tree.column("card_id", width=100, anchor="center")
    tree.column("name", width=150, anchor="center")
    tree.column("status", width=100, anchor="center")
    
    # Set column headings
    tree.heading("timestamp", text="Timestamp")
    tree.heading("card_id", text="Card ID")
    tree.heading("name", text="Name")
    tree.heading("status", text="Status")
    
    # Add scrollbars
    vsb = ttk.Scrollbar(tree_frame, orient="vertical", command=tree.yview)
    hsb = ttk.Scrollbar(tree_frame, orient="horizontal", command=tree.xview)
    tree.configure(yscrollcommand=vsb.set, xscrollcommand=hsb.set)
    
    # Pack layout for treeview and scrollbars
    vsb.pack(side=RIGHT, fill=Y)
    hsb.pack(side=BOTTOM, fill=X)
    tree.pack(side=LEFT, fill=BOTH, expand=True)
    
    # Load logs
    def load_logs():
        # Clear existing items
        for item in tree.get_children():
            tree.delete(item)
        
        # Load logs from file
        if os.path.exists(LOGS_FILE):
            try:
                with open(LOGS_FILE, "r") as f:
                    reader = csv.reader(f)
                    next(reader)  # Skip header
                    for i, row in enumerate(reader):
                        tree.insert("", "end", iid=i, values=row)
            except Exception as e:
                print(f"Error loading logs: {e}")
        
        # Schedule to refresh every 5 seconds
        parent.after(5000, load_logs)
    
    # Button frame - now using pack instead of grid
    btn_frame = Frame(logs_frame, bg=colors["white"])
    btn_frame.pack(fill=X, pady=10)
    
    # Refresh button
    refresh_btn = Button(btn_frame, text="Refresh", 
                      font=("Helvetica", 10),
                      bg=colors["primary"], fg=colors["white"],
                      command=load_logs)
    refresh_btn.pack(side=LEFT, padx=5)
    
    # Export button
    def export_logs():
        file_path = filedialog.asksaveasfilename(
            defaultextension=".csv",
            filetypes=[("CSV files", "*.csv"), ("All files", "*.*")],
            title="Export Logs"
        )
        if file_path:
            try:
                shutil.copy2(LOGS_FILE, file_path)
                messagebox.showinfo("Export Successful", f"Logs exported to {file_path}")
            except Exception as e:
                messagebox.showerror("Export Failed", f"Error exporting logs: {e}")
    
    export_btn = Button(btn_frame, text="Export to CSV", 
                      font=("Helvetica", 10),
                      bg=colors["secondary"], fg=colors["white"],
                      command=export_logs)
    export_btn.pack(side=LEFT, padx=5)
    
    # Clear logs button
    def clear_logs():
        if messagebox.askyesno("Clear Logs", "Are you sure you want to clear all logs?"):
            try:
                # Create new logs file with just the header
                with open(LOGS_FILE, "w", newline='') as f:
                    writer = csv.writer(f)
                    writer.writerow(["Timestamp", "Card ID", "Name", "Status"])
                
                # Refresh display
                load_logs()
                messagebox.showinfo("Logs Cleared", "All logs have been cleared.")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to clear logs: {e}")
    
    clear_btn = Button(btn_frame, text="Clear Logs", 
                     font=("Helvetica", 10),
                     bg=colors["error"], fg=colors["white"],
                     command=clear_logs)
    clear_btn.pack(side=LEFT, padx=5)
    
    # Load logs initially
    load_logs()

def setup_users_tab(parent):
    """Set up the user management tab"""
    # Create frame for users
    users_frame = Frame(parent, bg=colors["white"], padx=20, pady=10)
    users_frame.pack(fill=BOTH, expand=True)
    
    # Title
    title_lbl = Label(users_frame, text="User Management", 
                    font=("Helvetica", 14, "bold"), 
                    fg=colors["primary"], bg=colors["white"])
    title_lbl.pack(pady=10)
    
    # Create a container frame for the treeview and scrollbars
    tree_frame = Frame(users_frame, bg=colors["white"])
    tree_frame.pack(fill=BOTH, expand=True, pady=10)
    
    # Treeview for users
    columns = ("card_id", "name", "image")
    tree = ttk.Treeview(tree_frame, columns=columns, show="headings")
    
    # Configure column widths
    tree.column("card_id", width=150, anchor="center")
    tree.column("name", width=200, anchor="center")
    tree.column("image", width=250, anchor="center")
    
    # Set column headings
    tree.heading("card_id", text="Card ID")
    tree.heading("name", text="Name")
    tree.heading("image", text="Image Path")
    
    # Add scrollbars
    vsb = ttk.Scrollbar(tree_frame, orient="vertical", command=tree.yview)
    hsb = ttk.Scrollbar(tree_frame, orient="horizontal", command=tree.xview)
    tree.configure(yscrollcommand=vsb.set, xscrollcommand=hsb.set)
    
    # Pack layout for treeview and scrollbars
    vsb.pack(side=RIGHT, fill=Y)
    hsb.pack(side=BOTTOM, fill=X)
    tree.pack(side=LEFT, fill=BOTH, expand=True)
    
    # Load users
    def load_users_to_tree():
        # Clear existing items
        for item in tree.get_children():
            tree.delete(item)
        
        # Load users from global USERS dictionary
        for card_id, user_data in USERS.items():
            tree.insert("", "end", values=(card_id, user_data["name"], user_data["image"]))
    
    # Button frame - now using pack instead of grid
    btn_frame = Frame(users_frame, bg=colors["white"])
    btn_frame.pack(fill=X, pady=10)
    
    # Edit user function
    def edit_user():
        # Get selected item
        selected = tree.selection()
        if not selected:
            messagebox.showwarning("No Selection", "Please select a user to edit")
            return
            
        card_id = tree.item(selected[0], "values")[0]
        user = USERS.get(card_id)
        
        if not user:
            messagebox.showerror("Error", "User not found")
            return
        
        # Create edit dialog with improved vertical layout
        edit_window = Toplevel(root)
        edit_window.title("Edit User")
        edit_window.geometry("550x350")  # Reduced height since we removed preview
        edit_window.configure(bg=colors["bg"])
        edit_window.resizable(False, False)
        
        # Center the window
        edit_window.transient(root)
        edit_window.grab_set()
        
        # Simple frame for content with adjusted padding
        edit_frame = Frame(edit_window, bg=colors["white"], padx=30, pady=20)
        edit_frame.pack(fill=BOTH, expand=True, padx=20, pady=20)
        
        # Edit form
        title_lbl = Label(edit_frame, text=f"Edit User: {card_id}", 
                        font=("Helvetica", 18, "bold"), 
                        fg=colors["primary"], bg=colors["white"])
        title_lbl.pack(pady=(0, 20))
        
        # Name frame
        name_frame = Frame(edit_frame, bg=colors["white"])
        name_frame.pack(fill=X, pady=10)
        
        name_lbl = Label(name_frame, text="Name:", 
                       font=("Helvetica", 12), 
                       fg=colors["text"], bg=colors["white"], width=8, anchor="w")
        name_lbl.pack(side=LEFT, padx=5)
        
        name_var = StringVar(value=user["name"])
        name_entry = Entry(name_frame, font=("Helvetica", 12), width=25, textvariable=name_var)
        name_entry.pack(side=LEFT, padx=5)
        
        # Image frame
        image_frame = Frame(edit_frame, bg=colors["white"])
        image_frame.pack(fill=X, pady=10)
        
        image_lbl = Label(image_frame, text="Image:", 
                        font=("Helvetica", 12), 
                        fg=colors["text"], bg=colors["white"], width=8, anchor="w")
        image_lbl.pack(side=LEFT, padx=5)
        
        image_path_var = StringVar(value=user["image"])
        image_entry = Entry(image_frame, font=("Helvetica", 12), width=25, textvariable=image_path_var, state="readonly")
        image_entry.pack(side=LEFT, padx=5)
        
        # Browse button with better positioning
        def browse_image():
            file_path = filedialog.askopenfilename(
                title="Select Image",
                filetypes=[("Image files", "*.jpg *.jpeg *.png")]
            )
            if file_path:
                image_path_var.set(file_path)
        
        browse_btn = Button(image_frame, text="Browse", 
                          font=("Helvetica", 10),
                          bg=colors["secondary"], fg=colors["white"],
                          padx=5, pady=2,
                          command=browse_image)
        browse_btn.pack(side=LEFT, padx=5)
        
        # Status message
        status_var = StringVar()
        status_lbl = Label(edit_frame, textvariable=status_var, 
                         font=("Helvetica", 12), 
                         fg=colors["secondary"], bg=colors["white"])
        status_lbl.pack(pady=10)
        
        # Save button
        def save_changes():
            name = name_var.get().strip()
            image_path = image_path_var.get().strip()
            
            if not name:
                status_var.set("Please enter a name")
                return
            
            try:
                # Check if new image is selected
                if image_path != user["image"]:
                    # Copy new image to user_images directory
                    extension = os.path.splitext(image_path)[1]
                    new_image_name = f"{card_id}{extension}"
                    new_image_path = os.path.join(IMAGES_DIR, new_image_name)
                    
                    shutil.copy2(image_path, new_image_path)
                    
                    # Update user image path
                    USERS[card_id]["image"] = new_image_path
                
                # Update user name
                USERS[card_id]["name"] = name
                
                # Save to file
                save_users(USERS)
                
                # Close dialog and refresh list
                edit_window.destroy()
                load_users_to_tree()
                messagebox.showinfo("Success", f"User {name} updated successfully!")
                
            except Exception as e:
                status_var.set(f"Error updating user: {e}")
        
        # Button container - fixed layout with proper spacing
        btn_container = Frame(edit_frame, bg=colors["white"])
        btn_container.pack(side=BOTTOM, fill=X, pady=10)
        
        # Cancel button (add first so it appears on the right)
        cancel_btn = Button(btn_container, text="Cancel", 
                          font=("Helvetica", 12),
                          bg=colors["secondary"], fg=colors["white"],
                          padx=10, pady=5,
                          command=edit_window.destroy)
        cancel_btn.pack(side=LEFT, padx=5)
        
        # Save button
        save_btn = Button(btn_container, text="Save Changes", 
                        font=("Helvetica", 12),
                        bg=colors["success"], fg=colors["white"],
                        padx=10, pady=5,
                        command=save_changes)
        save_btn.pack(side=LEFT, padx=5)

    # Delete user function
    def delete_user():
        # Get selected item
        selected = tree.selection()
        if not selected:
            messagebox.showwarning("No Selection", "Please select a user to delete")
            return
            
        card_id = tree.item(selected[0], "values")[0]
        user = USERS.get(card_id)
        
        if not user:
            messagebox.showerror("Error", "User not found")
            return
        
        # Confirm deletion
        if messagebox.askyesno("Confirm Delete", f"Are you sure you want to delete user {user['name']}?"):
            try:
                # Remove user from dictionary
                del USERS[card_id]
                
                # Save to file
                save_users(USERS)
                
                # Refresh list
                load_users_to_tree()
                messagebox.showinfo("Success", "User deleted successfully!")
                
            except Exception as e:
                messagebox.showerror("Error", f"Failed to delete user: {e}")
    
    # Button styles
    button_style = {
        "font": ("Helvetica", 10),
        "fg": colors["white"],
        "width": 15
    }
    
    # Refresh button
    refresh_btn = Button(btn_frame, text="Refresh List", 
                      bg=colors["primary"],
                      command=load_users_to_tree,
                      **button_style)
    refresh_btn.pack(side=LEFT, padx=5)
    
    # Edit button
    edit_btn = Button(btn_frame, text="Edit User", 
                    bg=colors["secondary"],
                    command=edit_user,
                    **button_style)
    edit_btn.pack(side=LEFT, padx=5)
    
    # Delete button
    delete_btn = Button(btn_frame, text="Delete User", 
                      bg=colors["error"],
                      command=delete_user,
                      **button_style)
    delete_btn.pack(side=LEFT, padx=5)
    
    # Load users initially
    load_users_to_tree()  # THIS LINE WAS MISSING - Call the function to load users when the tab is created
# --- SERIAL CONNECTION AND APPLICATION STARTUP ---

# Initialize serial connection
try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Connected to {PORT} at {BAUD} baud")
except Exception as e:
    print(f"Serial connection error: {e}")
    messagebox.showerror("Connection Error", f"Failed to connect to RFID reader at {PORT}.\n\nError: {e}")
    # Continue without serial connection for UI testing
    ser = None

# Start the RFID reading thread if connection was successful
if ser is not None:
    thread = threading.Thread(target=read_loop, daemon=True)
    thread.start()

# Add admin user if not exists
if not os.path.exists(USERS_FILE):
    save_users(USERS)

current_state = "main_menu"
show_main_options()

# Start the mainloop
root.mainloop()
import tkinter as tk
from tkinter import messagebox, simpledialog, ttk
from datetime import datetime
import customtkinter as ctk
import sqlite3
import threading
import time
from tkcalendar import DateEntry
from reportlab.lib.pagesizes import letter
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Paragraph, Spacer
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib import colors
from reportlab.lib.units import inch
import os
import tempfile
import subprocess
import platform

class ModernSteelInventoryApp:
    def __init__(self, root):
        self.root = root
        self.root.title("AS Steel Mills - Inventory Management")
        self.root.geometry("1200x800")
        
        
        # Set modern theme
        ctk.set_appearance_mode("dark")
        ctk.set_default_color_theme("blue")
        
        # Configure root window
        self.root.configure(bg="#1a1a1a")
        
        # Animation variables
        self.current_screen = None
        self.animation_running = False
        
        # Database setup
        self.db_name = "steel_inventory.db"
        self.setup_database()
        
        self.parties = []
        self.load_parties()
        
        # Initialize main screen
        self.show_main_screen()

        self.center_window()

    def setup_database(self):
        self.conn = sqlite3.connect(self.db_name)
        self.cursor = self.conn.cursor()

        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS parties (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT UNIQUE NOT NULL
            )
        """)

        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS transactions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                party TEXT NOT NULL,
                action TEXT NOT NULL,
                date TEXT NOT NULL,
                weight INTEGER NOT NULL,
                car_no TEXT NOT NULL,
                towards_party TEXT,
                description TEXT
            )
        """)

        self.conn.commit()

    def load_parties(self):
        self.cursor.execute("SELECT name FROM parties")
        self.parties = [row[0] for row in self.cursor.fetchall()]

    def center_window(self):
        self.root.update_idletasks()
        width = 1200
        height = 800
        x = (self.root.winfo_screenwidth() // 2) - (width // 2)
        y = (self.root.winfo_screenheight() // 2) - (height // 2)
        self.root.geometry(f"{width}x{height}+{x}+{y}")

    def animate_fade_out(self, widget, callback=None):
        """Smooth fade out animation"""
        if self.animation_running:
            return
        
        self.animation_running = True
        alpha = 1.0
        
        def fade_step():
            nonlocal alpha
            alpha -= 0.05
            if alpha <= 0:
                widget.destroy()
                self.animation_running = False
                if callback:
                    callback()
            else:
                # Simulate opacity by adjusting colors
                widget.configure(fg_color=self.blend_colors("#2b2b2b", "#1a1a1a", 1-alpha))
                self.root.after(20, fade_step)
        
        fade_step()

    def animate_slide_in(self, widget, direction="left"):
        """Smooth slide in animation"""
        if direction == "left":
            widget.place(x=-widget.winfo_reqwidth(), y=0)
            target_x = 0
        else:
            widget.place(x=self.root.winfo_width(), y=0)
            target_x = 0
        
        def slide_step():
            current_x = widget.winfo_x()
            if abs(current_x - target_x) < 5:
                widget.place(x=target_x, y=0)
                widget.pack(fill="both", expand=True)
                widget.place_forget()
            else:
                new_x = current_x + (target_x - current_x) * 0.2
                widget.place(x=new_x, y=0)
                self.root.after(20, slide_step)
        
        slide_step()

    def blend_colors(self, color1, color2, ratio):
        """Blend two hex colors"""
        # Simple color blending for animation effect
        return color1 if ratio > 0.5 else color2

    def create_modern_button(self, parent, text, command, color="#007acc", hover_color="#005a9e", width=200, height=50):
        """Create a modern animated button"""
        button = ctk.CTkButton(
            parent,
            text=text,
            command=command,
            width=width,
            height=height,
            corner_radius=15,
            fg_color=color,
            hover_color=hover_color,
            font=("Segoe UI", 14, "bold"),
            border_width=2,
            border_color="#3d3d3d"
        )
        
        # Add hover animation
        def on_enter(event):
            button.configure(border_color="#007acc")
            
        def on_leave(event):
            button.configure(border_color="#3d3d3d")
            
        button.bind("<Enter>", on_enter)
        button.bind("<Leave>", on_leave)
        
        return button

    def create_modern_frame(self, parent, corner_radius=20):
        """Create a modern frame with gradient-like effect"""
        frame = ctk.CTkFrame(
            parent,
            corner_radius=corner_radius,
            fg_color="#2b2b2b",
            border_width=1,
            border_color="#404040"
        )
        return frame

    def show_main_screen(self):
        self.clear_screen_animated()
        
        # Main container
        main_container = self.create_modern_frame(self.root)
        main_container.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Header section with gradient effect
        header_frame = self.create_modern_frame(main_container, corner_radius=15)
        header_frame.pack(fill="x", padx=20, pady=20)
        
        # Company logo/title with modern typography
        title = ctk.CTkLabel(
            header_frame,
            text="AS STEEL MILLS",
            font=("Segoe UI", 36, "bold"),
            text_color="#ffffff"
        )
        title.pack(pady=30)
        
        subtitle = ctk.CTkLabel(
            header_frame,
            text="Advanced Inventory Management System",
            font=("Segoe UI", 16),
            text_color="#a0a0a0"
        )
        subtitle.pack(pady=(0, 30))
        
        # Stats section
        stats_frame = ctk.CTkFrame(main_container, fg_color="transparent")
        stats_frame.pack(fill="x", padx=20, pady=10)
        
        # Get quick stats
        self.cursor.execute("SELECT COUNT(*) FROM transactions WHERE action = 'IN'")
        in_count = self.cursor.fetchone()[0]
        
        self.cursor.execute("SELECT COUNT(*) FROM transactions WHERE action = 'OUT'")
        out_count = self.cursor.fetchone()[0]
        
        self.cursor.execute("SELECT COUNT(*) FROM parties")
        party_count = self.cursor.fetchone()[0]
        
        # Create stat cards
        stats = [
            ("Total IN", in_count, "#4bc879"),
            ("Total OUT", out_count, "#f87171"),
            ("Active Parties", party_count, "#60a5fa")
        ]
        
        for i, (label, value, color) in enumerate(stats):
            stat_card = self.create_modern_frame(stats_frame, corner_radius=12)
            stat_card.pack(side="left", fill="x", expand=True, padx=10)
            
            value_label = ctk.CTkLabel(
                stat_card,
                text=str(value),
                font=("Segoe UI", 24, "bold"),
                text_color=color
            )
            value_label.pack(pady=(15, 5))
            
            desc_label = ctk.CTkLabel(
                stat_card,
                text=label,
                font=("Segoe UI", 12),
                text_color="#a0a0a0"
            )
            desc_label.pack(pady=(0, 15))
        
        # Action buttons section
        buttons_frame = ctk.CTkFrame(main_container, fg_color="transparent")
        buttons_frame.pack(expand=True, fill="both", padx=20, pady=20)
        
        # Center the buttons
        center_frame = ctk.CTkFrame(buttons_frame, fg_color="transparent")
        center_frame.place(relx=0.5, rely=0.5, anchor="center")
        
        # Create modern action buttons
        enter_data_btn = self.create_modern_button(
            center_frame,
            "📝 ENTER NEW DATA",
            self.show_data_entry_screen,
            color="#0a9a3f",
            hover_color="#068032",
            width=250,
            height=60
        )
        enter_data_btn.pack(pady=15)
        
        open_ledger_btn = self.create_modern_button(
            center_frame,
            "📊 OPEN LEDGER",
            self.show_ledger_screen,
            color="#60a5fa",
            hover_color="#3b82f6",
            width=250,
            height=60
        )
        open_ledger_btn.pack(pady=15)
        
        # Add pulsing animation to buttons
        self.add_pulse_animation(enter_data_btn)
        self.add_pulse_animation(open_ledger_btn)

    def add_pulse_animation(self, button):
        """Add subtle pulse animation to buttons"""
        original_width = button.cget("width")
        
        def pulse():
            try:
                # Subtle size change
                button.configure(width=original_width + 5)
                self.root.after(500, lambda: button.configure(width=original_width))
                self.root.after(3000, pulse)  # Repeat every 3 seconds
            except:
                pass  # Button might be destroyed
        
        self.root.after(2000, pulse)  # Start after 2 seconds

    def show_data_entry_screen(self):
        self.clear_screen_animated()
        
        # Main container with modern styling
        main_container = self.create_modern_frame(self.root)
        main_container.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Header
        header = ctk.CTkLabel(
            main_container,
            text="📝 Data Entry",
            font=("Segoe UI", 28, "bold"),
            text_color="#ffffff"
        )
        header.pack(pady=20)
        
        # Scrollable frame for form
        scrollable_frame = ctk.CTkScrollableFrame(
            main_container,
            corner_radius=15,
            fg_color="#2b2b2b"
        )
        scrollable_frame.pack(fill="both", expand=True, padx=20, pady=10)
        
        # Party selection section
        self.create_form_section(scrollable_frame, "Party Information")
        
        party_frame = self.create_modern_frame(scrollable_frame)
        party_frame.pack(fill="x", padx=20, pady=10)
        
        party_label = ctk.CTkLabel(party_frame, text="Select Party:", font=("Segoe UI", 14, "bold"))
        party_label.pack(anchor="w", padx=20, pady=(15, 5))
        
        self.party_var = tk.StringVar()
        self.party_dropdown = ctk.CTkComboBox(
            party_frame,
            values=self.parties,
            variable=self.party_var,
            width=300,
            height=35,
            corner_radius=10,
            font=("Segoe UI", 12)
        )
        self.party_dropdown.pack(padx=20, pady=5)
        
        # Party management buttons
        party_btn_frame = ctk.CTkFrame(party_frame, fg_color="transparent")
        party_btn_frame.pack(pady=10)
        
        add_party_btn = self.create_modern_button(
            party_btn_frame, "➕ Add Party", self.add_party_popup,
            color="#22c55e", width=120, height=35
        )
        add_party_btn.pack(side="left", padx=5)
        
        remove_party_btn = self.create_modern_button(
            party_btn_frame, "➖ Remove Party", self.remove_party_popup,
            color="#ef4444", width=120, height=35
        )
        remove_party_btn.pack(side="left", padx=5)
        
        # Action selection
        self.create_form_section(scrollable_frame, "Transaction Type")
        
        action_frame = self.create_modern_frame(scrollable_frame)
        action_frame.pack(fill="x", padx=20, pady=10)
        
        self.action_var = tk.StringVar()
        
        action_btn_frame = ctk.CTkFrame(action_frame, fg_color="transparent")
        action_btn_frame.pack(pady=20)
        
        self.in_button = self.create_modern_button(
            action_btn_frame, "📥 IN", lambda: self.set_action("IN"),
            color="#22c55e", width=100, height=45
        )
        self.in_button.pack(side="left", padx=10)
        
        self.out_button = self.create_modern_button(
            action_btn_frame, "📤 OUT", lambda: self.set_action("OUT"),
            color="#ef4444", width=100, height=45
        )
        self.out_button.pack(side="left", padx=10)
        
        # Date selection
        self.create_form_section(scrollable_frame, "Date Selection")
        
        date_frame = self.create_modern_frame(scrollable_frame)
        date_frame.pack(fill="x", padx=20, pady=10)
        
        date_btn_frame = ctk.CTkFrame(date_frame, fg_color="transparent")
        date_btn_frame.pack(pady=15)
        
        self.current_date_button = self.create_modern_button(
            date_btn_frame, "📅 Current Date", self.use_current_date,
            width=150, height=35
        )
        self.current_date_button.pack(side="left", padx=10)
        
        self.custom_date_button = self.create_modern_button(
            date_btn_frame, "📝 Custom Date", self.use_custom_date,
            width=150, height=35
        )
        self.custom_date_button.pack(side="left", padx=10)
        
        self.custom_date_var = tk.StringVar()
        self.date_picker = DateEntry(
            date_frame,
            width=12,
            background='#2b2b2b',
            foreground='white',
            borderwidth=2,
            year=datetime.now().year,
            month=datetime.now().month,
            day=datetime.now().day,
            font=("Segoe UI", 12)
        )
        
        # Weight and Car No
        self.create_form_section(scrollable_frame, "Transaction Details")
        
        details_frame = self.create_modern_frame(scrollable_frame)
        details_frame.pack(fill="x", padx=20, pady=10)
        
        # Weight
        weight_label = ctk.CTkLabel(details_frame, text="Weight (kg):", font=("Segoe UI", 14, "bold"))
        weight_label.pack(anchor="w", padx=20, pady=(15, 5))
        
        self.weight_var = tk.StringVar()
        self.weight_entry = ctk.CTkEntry(
            details_frame,
            placeholder_text="Enter weight in kg",
            width=300,
            height=35,
            corner_radius=10,
            font=("Segoe UI", 12)
        )
        self.weight_entry.pack(padx=20, pady=5)
        
        # Car No
        car_label = ctk.CTkLabel(details_frame, text="Car Number:", font=("Segoe UI", 14, "bold"))
        car_label.pack(anchor="w", padx=20, pady=(15, 5))
        
        self.car_var = tk.StringVar()
        self.car_entry = ctk.CTkEntry(
            details_frame,
            placeholder_text="Enter car number",
            width=300,
            height=35,
            corner_radius=10,
            font=("Segoe UI", 12)
        )
        self.car_entry.pack(padx=20, pady=(5, 15))
        
                # Optional fields - Replace the existing optional fields section
        self.create_form_section(scrollable_frame, "Additional Information (Optional)")

        optional_frame = self.create_modern_frame(scrollable_frame)
        optional_frame.pack(fill="x", padx=20, pady=10)

        # Towards Party checkbox
        self.towards_checkbox = ctk.CTkCheckBox(
            optional_frame,
            text="Towards Party",
            font=("Segoe UI", 12),
            command=self.toggle_towards_entry
        )
        self.towards_checkbox.pack(pady=(15, 5))

        # Towards Party entry
        self.towards_var = tk.StringVar()
        self.towards_entry = ctk.CTkEntry(
            optional_frame,
            placeholder_text="Enter towards party name",
            width=300,
            height=35,
            corner_radius=10,
            font=("Segoe UI", 12)
        )

        # Description checkbox
        self.description_checkbox = ctk.CTkCheckBox(
            optional_frame,
            text="Description",
            font=("Segoe UI", 12),
            command=self.toggle_description_entry
        )
        self.description_checkbox.pack(pady=(15, 10))

        # Description entry
        self.description_var = tk.StringVar()
        self.description_entry = ctk.CTkEntry(
            optional_frame,
            placeholder_text="Enter description",
            width=300,
            height=35,
            corner_radius=10,
            font=("Segoe UI", 12)
        )

        
        # Action buttons
        action_frame = ctk.CTkFrame(main_container, fg_color="transparent")
        action_frame.pack(fill="x", padx=20, pady=20)
        
        submit_btn = self.create_modern_button(
            action_frame, "✅ SUBMIT", self.submit_data,
            color="#22c55e", hover_color="#16a34a", width=150, height=45
        )
        submit_btn.pack(side="right", padx=10)
        
        back_btn = self.create_modern_button(
            action_frame, "🔙 BACK", self.show_main_screen,
            color="#6b7280", hover_color="#4b5563", width=150, height=45
        )
        back_btn.pack(side="right", padx=10)

    def create_form_section(self, parent, title):
        """Create a modern form section header"""
        section_frame = ctk.CTkFrame(parent, fg_color="transparent", height=50)
        section_frame.pack(fill="x", padx=20, pady=(20, 0))
        
        title_label = ctk.CTkLabel(
            section_frame,
            text=title,
            font=("Segoe UI", 18, "bold"),
            text_color="#60a5fa"
        )
        title_label.pack(anchor="w", pady=10)
        
        # Add separator line
        separator = ctk.CTkFrame(section_frame, height=2, fg_color="#404040")
        separator.pack(fill="x", pady=(0, 10))

    def toggle_towards_entry(self):
        if self.towards_checkbox.get():
            self.towards_entry.pack(after=self.towards_checkbox, pady=(10, 10))
        else:
            self.towards_entry.pack_forget()

    def toggle_description_entry(self):
        if self.description_checkbox.get():
            self.description_entry.pack(after=self.description_checkbox, pady=(10, 15))
        else:
            self.description_entry.pack_forget()

    def set_action(self, action):
        self.action_var.set(action)
        if action == "IN":
            self.in_button.configure(fg_color="#16a34a", border_color="#22c55e")
            self.out_button.configure(fg_color="#007acc", border_color="#3d3d3d")
        elif action == "OUT":
            self.in_button.configure(fg_color="#007acc", border_color="#3d3d3d")
            self.out_button.configure(fg_color="#dc2626", border_color="#ef4444")

    def use_current_date(self):
        self.date_picker.pack_forget()  # Changed from custom_date_entry
        self.current_date_button.configure(fg_color="#16a34a", border_color="#22c55e")
        self.custom_date_button.configure(fg_color="#007acc", border_color="#3d3d3d")
        self.custom_date_var.set(datetime.now().strftime("%Y-%m-%d"))

    def use_custom_date(self):
        self.date_picker.pack(padx=20, pady=10)  # Changed from custom_date_entry
        self.current_date_button.configure(fg_color="#007acc", border_color="#3d3d3d")
        self.custom_date_button.configure(fg_color="#16a34a", border_color="#22c55e")
        # Remove the line: self.custom_date_var.set("")

    def submit_data(self):
        party = self.party_var.get().strip()
        action = self.action_var.get().strip()
        if self.current_date_button.cget("fg_color") == "#16a34a":
            date = datetime.now().strftime("%Y-%m-%d")
        else:
            date = self.date_picker.get_date().strftime("%Y-%m-%d")
        weight = self.weight_var.get().strip()
        car_no = self.car_var.get().strip()
        towards_party = self.towards_var.get().strip() if self.towards_checkbox.get() else "N/A"
        description = self.description_var.get().strip() if self.description_checkbox.get() else "N/A"

        if not (party and action and date and weight.isdigit() and car_no):
            self.show_modern_messagebox("⚠️ Missing Information", "Please fill all required fields correctly.", "warning")
            return

        self.cursor.execute("""
            INSERT INTO transactions (party, action, date, weight, car_no, towards_party, description)
            VALUES (?, ?, ?, ?, ?, ?, ?)
        """, (party, action, date, int(weight), car_no, towards_party, description))
        self.conn.commit()

        self.show_modern_messagebox("✅ Success", "Data submitted successfully!", "success")
        self.show_main_screen()

    def show_modern_messagebox(self, title, message, msg_type="info"):
        """Create a modern custom messagebox"""
        dialog = ctk.CTkToplevel(self.root)
        dialog.title(title)
        dialog.geometry("400x200")
        dialog.resizable(False, False)
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Center the dialog
        dialog.geometry("+%d+%d" % (self.root.winfo_rootx() + 400, self.root.winfo_rooty() + 300))
        
        # Color scheme based on message type
        colors = {
            "success": "#22c55e",
            "error": "#ef4444",
            "warning": "#f59e0b",
            "info": "#60a5fa"
        }
        
        color = colors.get(msg_type, "#60a5fa")
        
        # Main frame
        main_frame = ctk.CTkFrame(dialog, fg_color="#2b2b2b", corner_radius=15)
        main_frame.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Title
        title_label = ctk.CTkLabel(
            main_frame,
            text=title,
            font=("Segoe UI", 18, "bold"),
            text_color=color
        )
        title_label.pack(pady=(20, 10))
        
        # Message
        message_label = ctk.CTkLabel(
            main_frame,
            text=message,
            font=("Segoe UI", 12),
            text_color="#ffffff",
            wraplength=300
        )
        message_label.pack(pady=10)
        
        # OK button
        ok_btn = self.create_modern_button(
            main_frame, "OK", dialog.destroy,
            color=color, width=100, height=35
        )
        ok_btn.pack(pady=20)

    def add_party_popup(self):
        dialog = ctk.CTkInputDialog(
            text="Enter new party name:",
            title="Add Party"
        )
        new_party = dialog.get_input()
        
        if new_party and new_party not in self.parties:
            self.cursor.execute("INSERT INTO parties (name) VALUES (?)", (new_party,))
            self.conn.commit()
            self.load_parties()
            self.party_dropdown.configure(values=self.parties)
            self.show_modern_messagebox("✅ Success", f"Party '{new_party}' added successfully!", "success")

    def remove_party_popup(self):
        dialog = ctk.CTkInputDialog(
            text="Enter party name to remove:",
            title="Remove Party"
        )
        party_to_remove = dialog.get_input()
        
        if party_to_remove and party_to_remove in self.parties:
            self.cursor.execute("DELETE FROM parties WHERE name = ?", (party_to_remove,))
            self.conn.commit()
            self.load_parties()
            self.party_dropdown.configure(values=self.parties)
            self.show_modern_messagebox("✅ Success", f"Party '{party_to_remove}' removed successfully!", "success")

    def show_ledger_screen(self):
        self.clear_screen_animated()
        
        # Main container
        main_container = self.create_modern_frame(self.root)
        main_container.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Header
        header = ctk.CTkLabel(
            main_container,
            text="📊 Ledger Management",
            font=("Segoe UI", 28, "bold"),
            text_color="#ffffff"
        )
        header.pack(pady=20)
        
        # Options frame
        options_frame = ctk.CTkScrollableFrame(
            main_container,
            corner_radius=15,
            fg_color="#2b2b2b"
        )
        options_frame.pack(fill="both", expand=True, padx=20, pady=10)
        
        # Quick access section
        self.create_form_section(options_frame, "Quick Access")
        
        quick_frame = self.create_modern_frame(options_frame)
        quick_frame.pack(fill="x", padx=20, pady=10)
        
        quick_btn_frame = ctk.CTkFrame(quick_frame, fg_color="transparent")
        quick_btn_frame.pack(pady=20)
        
        in_btn = self.create_modern_button(
            quick_btn_frame, "📥 ALL IN TRANSACTIONS", lambda: self.view_ledger("IN"),
            color="#22c55e", width=200, height=45
        )
        in_btn.pack(side="left", padx=10)
        
        out_btn = self.create_modern_button(
            quick_btn_frame, "📤 ALL OUT TRANSACTIONS", lambda: self.view_ledger("OUT"),
            color="#ef4444", width=200, height=45
        )
        out_btn.pack(side="left", padx=10)
        
        # Party-wise ledger
        if self.parties:
            self.create_form_section(options_frame, "Party-wise Ledger")
            
            party_frame = self.create_modern_frame(options_frame)
            party_frame.pack(fill="x", padx=20, pady=10)
            
            # Create party buttons in a grid
            btn_frame = ctk.CTkFrame(party_frame, fg_color="transparent")
            btn_frame.pack(pady=20, fill="x")
            
            for i, party in enumerate(self.parties):
                row = i // 3
                col = i % 3
                
                party_btn = self.create_modern_button(
                    btn_frame, f"👤 {party}", lambda p=party: self.view_ledger(p),
                    color="#60a5fa", width=180, height=40
                )
                party_btn.grid(row=row, column=col, padx=10, pady=5, sticky="ew")
            
            # Configure grid weights
            for i in range(3):
                btn_frame.grid_columnconfigure(i, weight=1)
        
        # Back button
        back_btn = self.create_modern_button(
            main_container, "🔙 BACK TO MAIN", self.show_main_screen,
            color="#6b7280", hover_color="#4b5563", width=200, height=45
        )
        back_btn.pack(pady=20)

    def view_ledger(self, ledger_type):
        """Create modern ledger view window with filters and print option"""
        ledger_window = ctk.CTkToplevel(self.root)
        ledger_window.title(f"{ledger_type} Ledger")
        ledger_window.geometry("1200x700")
        ledger_window.transient(self.root)
        
        # Main frame
        main_frame = ctk.CTkFrame(ledger_window, fg_color="#1a1a1a")
        main_frame.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Header
        header = ctk.CTkLabel(
            main_frame,
            text=f"📋 {ledger_type} Ledger",
            font=("Segoe UI", 24, "bold"),
            text_color="#ffffff"
        )
        header.pack(pady=20)
        
        # Filter frame
        filter_frame = ctk.CTkFrame(main_frame, fg_color="#2b2b2b", corner_radius=15)
        filter_frame.pack(fill="x", padx=20, pady=10)
        
        filter_title = ctk.CTkLabel(
            filter_frame,
            text="🔍 Filters",
            font=("Segoe UI", 16, "bold"),
            text_color="#60a5fa"
        )
        filter_title.pack(pady=(10, 5))
        
        # Filter controls
        filter_controls = ctk.CTkFrame(filter_frame, fg_color="transparent")
        filter_controls.pack(fill="x", padx=20, pady=10)
        
        # Date filters
        date_frame = ctk.CTkFrame(filter_controls, fg_color="transparent")
        date_frame.pack(side="left", padx=10)
        
        ctk.CTkLabel(date_frame, text="From Date:", font=("Segoe UI", 12)).pack()
        self.from_date = DateEntry(date_frame, width=12, background='#2b2b2b', 
                                foreground='white', borderwidth=2)
        self.from_date.pack(pady=2)
        
        ctk.CTkLabel(date_frame, text="To Date:", font=("Segoe UI", 12)).pack()
        self.to_date = DateEntry(date_frame, width=12, background='#2b2b2b', 
                                foreground='white', borderwidth=2)
        self.to_date.pack(pady=2)
        
        # Car number filter
        car_frame = ctk.CTkFrame(filter_controls, fg_color="transparent")
        car_frame.pack(side="left", padx=20)
        
        ctk.CTkLabel(car_frame, text="Car Number:", font=("Segoe UI", 12)).pack()
        self.car_filter = ctk.CTkEntry(car_frame, placeholder_text="Enter car number", width=150)
        self.car_filter.pack(pady=2)
        
        # Filter buttons
        btn_frame = ctk.CTkFrame(filter_controls, fg_color="transparent")
        btn_frame.pack(side="left", padx=20)
        
        apply_filter_btn = self.create_modern_button(
            btn_frame, "Apply Filter", lambda: self.apply_filters(ledger_type, table),
            color="#22c55e", width=120, height=35
        )
        apply_filter_btn.pack(pady=5)
        
        clear_filter_btn = self.create_modern_button(
            btn_frame, "Clear All", lambda: self.clear_filters(ledger_type, table),
            color="#ef4444", width=120, height=35
        )
        clear_filter_btn.pack(pady=5)
        
        # Action buttons
        action_frame = ctk.CTkFrame(main_frame, fg_color="transparent")
        action_frame.pack(side="bottom", fill="x", padx=20, pady=10)
        
        print_btn = self.create_modern_button(
            action_frame, "🖨️ PRINT PDF", lambda: self.print_ledger_pdf(ledger_type, table),
            color="#60a5fa", hover_color="#3b82f6", width=150, height=40
        )
        print_btn.pack(side="left", padx=10)
        
        close_btn = self.create_modern_button(
            action_frame, "❌ CLOSE", ledger_window.destroy,
            color="#ef4444", hover_color="#dc2626", width=150, height=40
        )
        close_btn.pack(side="right", padx=10)

        # Summary frame with enhanced stats
        summary_frame = ctk.CTkFrame(main_frame, fg_color="#2b2b2b", corner_radius=15)
        summary_frame.pack(side="bottom", fill="x", padx=20, pady=5)
        

        # Table frame
        table_frame = ctk.CTkFrame(main_frame, fg_color="#2b2b2b", corner_radius=15)
        table_frame.pack(fill="both", expand=True, padx=20, pady=10)
        
        # Create treeview with modern styling
        style = ttk.Style()
        style.theme_use("clam")
        style.configure("Treeview", 
                    background="#2b2b2b",
                    foreground="#ffffff",
                    rowheight=30,
                    fieldbackground="#2b2b2b",
                    font=("Segoe UI", 10))
        style.configure("Treeview.Heading",
                    background="#404040",
                    foreground="#ffffff",
                    font=("Segoe UI", 11, "bold"))
        style.map("Treeview",
                background=[("selected", "#007acc")])
        
        cols = ["Party", "Action", "Date", "Weight (kg)", "Car No.", "Towards Party", "Description"]
        table = ttk.Treeview(table_frame, columns=cols, show="headings", style="Treeview")
        
        for col in cols:
            table.heading(col, text=col)
            table.column(col, anchor="center", width=120)
        
        # Add scrollbars
        v_scrollbar = ttk.Scrollbar(table_frame, orient="vertical", command=table.yview)
        h_scrollbar = ttk.Scrollbar(table_frame, orient="horizontal", command=table.xview)
        table.configure(yscrollcommand=v_scrollbar.set, xscrollcommand=h_scrollbar.set)
        
        # Pack table and scrollbars
        table.pack(side="left", fill="both", expand=True, padx=10, pady=10)
        v_scrollbar.pack(side="right", fill="y")
        h_scrollbar.pack(side="bottom", fill="x")
        
        # Load initial data
        self.load_ledger_data(ledger_type, table)
        
        self.summary_label = ctk.CTkLabel(
            summary_frame,
            text="Loading summary...",
            font=("Segoe UI", 14, "bold"),
            text_color="#60a5fa"
        )
        self.summary_label.pack(pady=15)
        
        # Update summary
        self.update_summary(ledger_type, table)
        

    def apply_filters(self, ledger_type, table):
        """Apply filters to the ledger view"""
        # Clear existing data
        for item in table.get_children():
            table.delete(item)
        
        # Build query with filters
        base_query = "SELECT party, action, date, weight, car_no, towards_party, description FROM transactions WHERE "
        conditions = []
        params = []
        
        # Ledger type condition
        if ledger_type in ("IN", "OUT"):
            conditions.append("action = ?")
            params.append(ledger_type)
        else:
            conditions.append("party = ?")
            params.append(ledger_type)
        
        # Date filters
        from_date = self.from_date.get_date().strftime("%Y-%m-%d")
        to_date = self.to_date.get_date().strftime("%Y-%m-%d")
        conditions.append("date BETWEEN ? AND ?")
        params.extend([from_date, to_date])
        
        # Car filter
        car_filter = self.car_filter.get().strip()
        if car_filter:
            conditions.append("car_no LIKE ?")
            params.append(f"%{car_filter}%")
        
        # Execute query
        query = base_query + " AND ".join(conditions) + " ORDER BY date DESC"
        self.cursor.execute(query, params)
        rows = self.cursor.fetchall()
        
        # Populate table
        for i, row in enumerate(rows):
            tags = ("evenrow",) if i % 2 == 0 else ("oddrow",)
            table.insert('', 'end', values=row, tags=tags)
        
        table.tag_configure("evenrow", background="#333333")
        table.tag_configure("oddrow", background="#2b2b2b")
        
        # Update summary
        self.update_summary(ledger_type, table, filtered=True)

    def clear_filters(self, ledger_type, table):
        """Clear all filters and reload data"""
        # Reset filter controls
        self.from_date.set_date(datetime.now().replace(day=1))  # First day of month
        self.to_date.set_date(datetime.now())  # Today
        self.car_filter.delete(0, 'end')
        
        # Reload all data
        self.load_ledger_data(ledger_type, table)
        self.update_summary(ledger_type, table)

    def load_ledger_data(self, ledger_type, table):
        """Load initial ledger data"""
        # Clear existing data
        for item in table.get_children():
            table.delete(item)
        
        # Fetch data
        if ledger_type in ("IN", "OUT"):
            self.cursor.execute("SELECT party, action, date, weight, car_no, towards_party, description FROM transactions WHERE action = ? ORDER BY date DESC", (ledger_type,))
        else:
            self.cursor.execute("SELECT party, action, date, weight, car_no, towards_party, description FROM transactions WHERE party = ? ORDER BY date DESC", (ledger_type,))
        
        rows = self.cursor.fetchall()
        
        # Populate table
        for i, row in enumerate(rows):
            tags = ("evenrow",) if i % 2 == 0 else ("oddrow",)
            table.insert('', 'end', values=row, tags=tags)
        
        table.tag_configure("evenrow", background="#333333")
        table.tag_configure("oddrow", background="#2b2b2b")

    def update_summary(self, ledger_type, table, filtered=False):
        """Update summary statistics"""
        # Get current table data
        items = table.get_children()
        
        if not items:
            self.summary_label.configure(text="No transactions found")
            return
        
        total_transactions = len(items)
        total_weight = 0
        in_weight = 0
        out_weight = 0
        in_count = 0
        out_count = 0
        
        for item in items:
            values = table.item(item)['values']
            action = values[1]  # Action column
            weight = int(values[3])  # Weight column
            
            total_weight += weight
            if action == "IN":
                in_weight += weight
                in_count += 1
            elif action == "OUT":
                out_weight += weight
                out_count += 1
        
        # Create summary text
        if ledger_type not in ("IN", "OUT"):  # Party-specific ledger
            balance = in_weight - out_weight
            balance_text = f"Balance: {balance:,} kg"
            if balance > 0:
                balance_text += " (Credit)"
            elif balance < 0:
                balance_text += " (Debit)"
            else:
                balance_text += " (Balanced)"
            
            summary_text = f"Total: {total_transactions} transactions | IN: {in_count} ({in_weight:,} kg) | OUT: {out_count} ({out_weight:,} kg) | {balance_text}"
        else:
            summary_text = f"Total Transactions: {total_transactions} | Total Weight: {total_weight:,} kg"
        
        if filtered:
            summary_text = f"[FILTERED] {summary_text}"
        
        self.summary_label.configure(text=summary_text)

    def print_ledger_pdf(self, ledger_type, table):
        """Generate and open PDF report"""
        try:
            # Get current table data
            items = table.get_children()
            if not items:
                self.show_modern_messagebox("⚠️ No Data", "No data to print!", "warning")
                return
            
            # Create temporary PDF file
            temp_file = tempfile.NamedTemporaryFile(delete=False, suffix='.pdf')
            doc = SimpleDocTemplate(temp_file.name, pagesize=letter)
            
            # Styles
            styles = getSampleStyleSheet()
            title_style = ParagraphStyle(
                'CustomTitle',
                parent=styles['Heading1'],
                fontSize=18,
                spaceAfter=30,
                alignment=1  # Center alignment
            )
            
            # Build PDF content
            story = []
            
            # Title
            title = Paragraph(f"AS Steel Mills - {ledger_type} Ledger Report", title_style)
            story.append(title)
            story.append(Spacer(1, 12))
            
            # Date
            date_para = Paragraph(f"Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}", styles['Normal'])
            story.append(date_para)
            story.append(Spacer(1, 12))
            
            # Summary
            summary_text = self.summary_label.cget("text")
            summary_para = Paragraph(f"Summary: {summary_text}", styles['Normal'])
            story.append(summary_para)
            story.append(Spacer(1, 20))
            
            # Table headers
            headers = ["Party", "Action", "Date", "Weight (kg)", "Car No.", "Towards Party", "Description"]
            data = [headers]
            
            # Table data
            for item in items:
                values = list(table.item(item)['values'])
                data.append(values)
            
            # Create table
            pdf_table = Table(data)
            pdf_table.setStyle(TableStyle([
                ('BACKGROUND', (0, 0), (-1, 0), colors.grey),
                ('TEXTCOLOR', (0, 0), (-1, 0), colors.whitesmoke),
                ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
                ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
                ('FONTSIZE', (0, 0), (-1, 0), 10),
                ('BOTTOMPADDING', (0, 0), (-1, 0), 12),
                ('BACKGROUND', (0, 1), (-1, -1), colors.beige),
                ('GRID', (0, 0), (-1, -1), 1, colors.black),
                ('FONTSIZE', (0, 1), (-1, -1), 8),
            ]))
            
            story.append(pdf_table)
            
            # Build PDF
            doc.build(story)
            temp_file.close()
            
            # Open PDF
            if platform.system() == 'Darwin':  # macOS
                subprocess.call(('open', temp_file.name))
            elif platform.system() == 'Windows':  # Windows
                os.startfile(temp_file.name)
            else:  # Linux
                subprocess.call(('xdg-open', temp_file.name))
            
            self.show_modern_messagebox("✅ Success", "PDF report generated and opened!", "success")
            
        except Exception as e:
            self.show_modern_messagebox("❌ Error", f"Failed to generate PDF: {str(e)}", "error")

    def clear_screen_animated(self):
        """Clear screen with fade animation"""
        if not self.animation_running:
            for widget in self.root.winfo_children():
                widget.destroy()

    def __del__(self):
        """Cleanup database connection"""
        if hasattr(self, 'conn'):
            self.conn.close()

# Enhanced application with loading screen
class LoadingScreen:
    def __init__(self, root):
        self.root = root
        self.root.title("AS Steel Mills - Loading...")
        self.root.geometry("600x400")
        self.root.configure(bg="#1a1a1a")
        
        self.center_window()
        
        # Remove window decorations for splash effect
        self.root.overrideredirect(True)
        
        # Create loading frame
        self.loading_frame = ctk.CTkFrame(
            self.root,
            corner_radius=20,
            fg_color="#2b2b2b",
            border_width=2,
            border_color="#007acc"
        )
        self.loading_frame.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Logo/Title
        title = ctk.CTkLabel(
            self.loading_frame,
            text="AS STEEL MILLS",
            font=("Segoe UI", 32, "bold"),
            text_color="#ffffff"
        )
        title.pack(pady=(50, 20))
        
        subtitle = ctk.CTkLabel(
            self.loading_frame,
            text="Inventory Management System",
            font=("Segoe UI", 16),
            text_color="#a0a0a0"
        )
        subtitle.pack(pady=(0, 30))
        
        # Loading bar
        self.progress = ctk.CTkProgressBar(
            self.loading_frame,
            width=400,
            height=20,
            corner_radius=10,
            progress_color="#007acc"
        )
        self.progress.pack(pady=20)
        self.progress.set(0)
        
        # Loading text
        self.loading_text = ctk.CTkLabel(
            self.loading_frame,
            text="Initializing application...",
            font=("Segoe UI", 12),
            text_color="#a0a0a0"
        )
        self.loading_text.pack(pady=10)
        
        # Version info
        version_label = ctk.CTkLabel(
            self.loading_frame,
            text="Version 2.0 - Modern Edition",
            font=("Segoe UI", 10),
            text_color="#666666"
        )
        version_label.pack(side="bottom", pady=20)
        
        # Start loading animation
        self.animate_loading()

    
    def center_window(self):
        self.root.update_idletasks()
        width = 600
        height = 400
        x = (self.root.winfo_screenwidth() // 2) - (width // 2)
        y = (self.root.winfo_screenheight() // 2) - (height // 2)
        self.root.geometry(f"{width}x{height}+{x}+{y}")

    def animate_loading(self):
        """Animate the loading process"""
        steps = [
            ("Connecting to database...", 0.2),
            ("Loading party information...", 0.4),
            ("Initializing user interface...", 0.6),
            ("Applying modern theme...", 0.8),
            ("Ready to launch!", 1.0)
        ]
        
        def update_progress(step_index=0):
            if step_index < len(steps):
                text, progress = steps[step_index]
                self.loading_text.configure(text=text)
                self.progress.set(progress)
                self.root.after(800, lambda: update_progress(step_index + 1))
            else:
                self.root.after(500, self.launch_main_app)
        
        update_progress()
    
    def launch_main_app(self):
        """Launch the main application"""
        self.root.destroy()
        
        # Create main application window
        main_root = ctk.CTk()
        main_root.title("AS Steel Mills - Inventory Management")
        main_root.geometry("1200x800")
        
        # Launch main app
        app = ModernSteelInventoryApp(main_root)
        main_root.mainloop()

if __name__ == "__main__":
    # Show loading screen first
    loading_root = ctk.CTk()
    loading_app = LoadingScreen(loading_root)
    loading_root.mainloop()
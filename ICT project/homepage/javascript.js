// Newsletter Subscription Button
function validateEmail() {
    const email = document.getElementById("email").value.trim(); 
    const errorMessage = document.getElementById("error-message");

    if (email === "") {
        errorMessage.textContent = "This field cannot be left blank.";
    }
    else if (!email.includes("@")) {
        errorMessage.textContent = "Please include an '@' in the email address.";
    }
    else {
        errorMessage.textContent = "";
        window.open("../newsletter subscription/newlettersubscrib.html", "_self")
    }
}

// Scroll up
document.addEventListener("DOMContentLoaded", () => {
    const scrollUpBtn = document.getElementById("scrollUpBtn");

    window.addEventListener("scroll", () => {
        scrollUpBtn.style.display = window.scrollY > 300 ? "block" : "none";
    });

    scrollUpBtn.addEventListener("click", () => {
        window.scrollTo({ top: 0, behavior: "smooth" });
    });
});
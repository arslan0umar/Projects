document.addEventListener("DOMContentLoaded", () => {
    const scrollUpBtn = document.getElementById("scrollUpBtn");

    // Show or hide the button on scroll
    window.addEventListener("scroll", () => {
        scrollUpBtn.style.display = window.scrollY > 300 ? "block" : "none";
    });

    // Scroll to the top when the button is clicked
    scrollUpBtn.addEventListener("click", () => {
        window.scrollTo({ top: 0, behavior: "smooth" });
    });
});

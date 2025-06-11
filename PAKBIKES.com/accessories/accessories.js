document.addEventListener("DOMContentLoaded", () => {
    const scrollUpBtn = document.getElementById("scrollUpBtn");

    window.addEventListener("scroll", () => {
        scrollUpBtn.style.display = window.scrollY > 300 ? "block" : "none";
    });

    scrollUpBtn.addEventListener("click", () => {
        window.scrollTo({ top: 0, behavior: "smooth" });
    });
});

window.addEventListener("DOMContentLoaded", function () {
    const scrollUpBtn = document.getElementById("scrollUpBtn");
    window.addEventListener("scroll", () => {
        scrollUpBtn.style.display = window.scrollY > 300 ? "block" : "none";
    });
    scrollUpBtn.addEventListener("click", () => {
        window.scrollTo({ top: 0, behavior: "smooth" });
    });
    const button = document.getElementById("submit");
    button.addEventListener("click", (event) => {
        event.preventDefault(); 
        const input1 = document.getElementById("email").value; 
        const input2 = document.getElementById("message").value; 
        if (input1 !== "" && input2 !== "") {
            alert("Thank You!");
        } else {
            alert("Please fill in both the email and message fields.");
        }
    });
});


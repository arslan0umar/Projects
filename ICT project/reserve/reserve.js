document.addEventListener("DOMContentLoaded", () => {
 
  const submitButton = document.getElementById("submitButton");
  submitButton.addEventListener("click", () => {
      const name = document.getElementById("name").value.trim();
      const email = document.getElementById("email").value.trim();
      const mobileNumber = document.getElementById("mobileNumber").value.trim();
      const numItems = document.getElementById("numItems").value.trim();
      const message = document.getElementById("importantMessage").value.trim();

      if (!name || !email || !mobileNumber || !numItems) {
          alert("Please fill out all required fields.");
          return;
      }

      alert(
        "Reservation Details:\n\n" +
        "Name: " + name + "\n" +
        "Email: " + email + "\n" +
        "Mobile Number: " + mobileNumber + "\n" +
        "Number of Items: " + numItems + "\n" +
        "Message: " + (message || "No message provided")
      );
  });
  const scrollUpBtn = document.getElementById("scrollUpBtn");

  window.addEventListener("scroll", () => {
      scrollUpBtn.style.display = window.scrollY > 300 ? "block" : "none";
  });

  scrollUpBtn.addEventListener("click", () => {
      window.scrollTo({ top: 0, behavior: "smooth" });
  });
});

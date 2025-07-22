const tempSpan = document.getElementById("temp");
const humiditySpan = document.getElementById("humidity");
const thresholdInput = document.getElementById("threshold");
const setButton = document.getElementById("setThreshold");
const resetButton = document.getElementById("resetAlert");

let apiBase = ""; // Leave blank if served by ESP32 directly, or use IP like 'http://192.168.1.100'

// Fetch current readings
async function updateData() {
  try {
    const res = await fetch(`${apiBase}/api/data`);
    const data = await res.json();
    tempSpan.textContent = data.temp;
    humiditySpan.textContent = data.humidity;
  } catch {
    tempSpan.textContent = "--";
    humiditySpan.textContent = "--";
  }
}

setButton.addEventListener("click", async () => {
  const val = parseFloat(thresholdInput.value);
  if (!isNaN(val)) {
    await fetch(`${apiBase}/api/threshold`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ threshold: val }),
    });
  }
});

resetButton.addEventListener("click", async () => {
  await fetch(`${apiBase}/api/reset`, { method: "POST" });
});

setInterval(updateData, 5000); // Update every 5s
updateData();

// PWA registration
if ("serviceWorker" in navigator) {
  navigator.serviceWorker.register("service-worker.js");
}

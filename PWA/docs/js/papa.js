/**
 * Resets all the papa calibration data to defaults.
 * Requests new information to display.
 * Between each writeValue, wait 10 ms to prevent the BLE
 * stack from backing up.
 */
async function papa_reset() {
  var response = confirm("Delete all configuration data?");
  if (response == true) {
    await characteristic.writeValue(encoder.encode("PAPA"));
    await new Promise(resolve => setTimeout(resolve, 10));
  }
}

/**
 * Changes the temperature coefficient of the papa device
 * 0.019 is default.
 */
async function papa_set_temp_coefficient() {
  var papa = prompt("New temperature coefficient?", "0.019");
  if (papa != null) {
    await characteristic.writeValue(encoder.encode("PAPA " + papa));
  }
}

/**
 * Changes the temperature constant of the papa device
 * 25 is default.
 */
async function papa_set_temp_constant() {
  var papa = prompt("New temperature constant to adjust readings to in C?", "25");
  if (papa != null) {
    await characteristic.writeValue(encoder.encode("PAPA " + papa));
  }
}

/**
 * Shows the papa Configuration dialog
 */
async function papa_config() {
  var modal = document.querySelector(".papa_config_modal");
  var html = document.querySelector("html");
  modal.classList.add("is-active");
  html.classList.add("is-clipped");

  modal
    .querySelector(".papa_modal_close")
    .addEventListener("click", function(e) {
      e.preventDefault();
      modal.classList.remove("is-active");
      html.classList.remove("is-clipped");
    });
  await characteristic.writeValue(encoder.encode("PAPA"));
  await new Promise(resolve => setTimeout(resolve, 10));
 
}
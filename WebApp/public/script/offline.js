// Our navbar has a class name called "navbar"
var navbarElement = document.querySelector('.navbar');

// Once the DOM is loaded, check for connectivity
document.addEventListener('DOMContentLoaded', function (event) {
    if (!navigator.onLine) {
        goOffline();
    }

    //Offline Event
    function goOffline() {
        // Change the color of navbar to #fafafa when offline [1]

        document.documentElement.style
            .setProperty('--main-color', 'var(--offline-color)');

        document.getElementById('bblogo').src = "./asset/bluebird-logo-big-offline.svg";

        // Add the snackbar to show when offline [2]
        document.getElementById("snackbar").innerHTML = "You are offline ⚡️";
        var snackbar = document.getElementById("snackbar");

        snackbar.style.background = 'var(--offline-color)';
        // Add the "show" class to div
        snackbar.className = "show";
        // After 5 seconds, remove the show class from div
        setTimeout(function () {
            snackbar.className = snackbar.className.replace("show", "");
        }, 3000);
    }

    // Online Event
    window.addEventListener("online", function () {
        // Change the color of navbar to #fff when online [1]

        document.documentElement.style
            .setProperty('--main-color', '#715ee4');

        document.getElementById('bblogo').src = "./asset/bluebird-logo-big.svg";

        // Add the snackbar to show we're back online [2]
        document.getElementById("snackbar").innerHTML = "You're back online  🙌 🎉";
        var snackbar = document.getElementById("snackbar");

        // Couldn't help myself 😬 Added confetti 🎉
        var duration = 1 * 1000; // One second
        var end = Date.now() + duration;

        (function frame() {
            // launch a few confetti from the left edge
            confetti({
                particleCount: 7,
                angle: 60,
                spread: 55,
                origin: {
                    x: 0
                }
            });
            // and launch a few from the right edge
            confetti({
                particleCount: 7,
                angle: 120,
                spread: 55,
                origin: {
                    x: 1
                }
            });

            // keep going until we are out of time
            if (Date.now() < end) {
                requestAnimationFrame(frame);
            }
        }());

        snackbar.style.background = 'var(--main-color)';
        // Add the "show" class to div
        snackbar.className = "show";
        // After 5 seconds, remove the show class from div
        setTimeout(function () {
            snackbar.className = snackbar.className.replace("show", "");
        }, 3000);
    });

    window.addEventListener("offline", function () {
        goOffline();
    });


});
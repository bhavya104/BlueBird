var CACHE_NAME = 'bluebird-cache-v2';
var urlsToCache = [
  '/',
  '/style/style.css',
  '/style/snackbar.css',
  '/script/pouchdb-7.2.1.js',
  '/script/app.js',
  '/script/offline.js',
  '/manifest.json',
  '/asset/bluebird-logo.png',
  '/asset/bluebird-logo-big.svg',
  '/asset/bluebird-logo-big-offline.svg'
];

self.addEventListener('install', function (event) {
  // Perform install steps
  event.waitUntil(
    caches.open(CACHE_NAME)
    .then(function (cache) {
      console.log('Opened cache');
      return cache.addAll(urlsToCache);
    })
  );
});

self.addEventListener('fetch', function(event) {
  event.respondWith(
    caches.match(event.request).then(function(response) {
      return response || fetch(event.request);
    })
  );
});
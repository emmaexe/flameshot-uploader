# flameshot-uploader

A C++ app for expanding flameshot's screenshot uploading capabilities.

I will clean up this file later, but for now here is some key info:

App currently only works with chibisafe, will add more functionality later.

App depends on:

- nlohmann_json - json library
- curlpp - c++ libcurl wrapper
- clip - library for interacting with the clipboard, requires:
  - libx11-dev
  - libpng-dev (optional for now)
- a flameshot install (can be a flatpak, does not matter)

Config file (Create if it does not exist, located at ``~/.config/flameshot-uploader/config.json``):

- version - the version of the app/config.
- server - the url where the post request should be sent (e.g. ``https://chibisafe.moe/api/upload``)
- apikey - your chibisafe api key
- command - The command you want to use. If you installed via flatpak, it should be ``flatpak run org.flameshot.Flameshot gui -r`` otherwise it should be ``flameshot gui -r``

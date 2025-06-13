# GotifyClientApp

A C application to interact with a [Gotify](https://gotify.net/) server, fetch application data, and manage notification images. Designed for Linux and Windows, it leverages cURL and system notification libraries to deliver seamless integration with Gotify's API.

## Features

- Fetches application information from a Gotify server.
- Downloads and caches notification images locally.
- Handles both Linux (`~/.cache/`) and Windows cache directories.
- Uses cURL for HTTP(S) requests (if available).
- Integrates with system notification libraries (libnotify) and websockets.

## Getting Started

### Prerequisites

- **Gotify server URL and Token**: Configure these in `GotifyClientApp.conf`.
- **Dependencies**:
  - `libcurl`
  - `libnotify`
  - `libwebsockets`
  - `libssl`
  - `libcrypto`
  - `libuv`
  - `libcap`
  - C compiler with C11 support

### Configuration

Create a `GotifyClientApp.conf` file in the project root:

```
URL=https://gotify.srv:8080/
Token=YOUR_GOTIFY_TOKEN
```

### Building

This project uses CMake:

```bash
mkdir build
cd build
cmake ..
make
```

On Windows, use your preferred CMake and build setup.

### Running

```bash
./GotifyClientApp
```

## Project Structure

- `src/`: C source files (main logic, HTTP, file handling).
- `CMakeLists.txt`: Build configuration.
- `GotifyClientApp.conf`: Server URL and token.

## Example Usage

- Fetches application list and images from your Gotify server.
- Stores notification images in the system cache directory for reuse.

## License

AGPL-3.0  
Copyright (c) 2024 Mohamed-Ismail MJRI

See [`src/main.h`](https://github.com/medismail/gotify_client_app/blob/main/src/main.h) and LICENSE for more details.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/my-feature`)
3. Commit your changes
4. Push to the branch (`git push origin feature/my-feature`)
5. Open a pull request

---

# Planned improvements

## Refresh application images on WebSocket reconnection

**Status:** Proposed; not implemented.  
**Target branch:** `feature/channel-notification-images`  
**Scope:** Small follow-up change to existing Gotify client connection flow.

### Problem

The client fetches `/application` and downloads application icons once during startup (currently in `src/main.c`, before the WebSocket reconnect loop). If an image download fails, that application's cached image path is empty for the rest of the process lifetime. A manual restart retries the download, but a normal WebSocket reconnection does not.

### Proposed change

- Extract the existing `/application` GET and response-state cleanup into a reusable function.
- Call it before the initial WebSocket connection **and** before subsequent connection attempts, reusing the existing reconnect/backoff loop in `main.c`.
- Reset `rest_done`, `rest_failed`, `rest_http_status`, `rest_response`, and `rest_response_len` safely for every request; handle an unsuccessful GET without preventing normal reconnect attempts.
- Reuse `get_applications_data()` and `get_image()` and the existing local image cache.
- Log refresh success/failure without logging Gotify tokens.
- Keep the current Windows tray/WIC notification implementation and Linux behavior intact; no new dependency or notification API is required.

### Trade-offs and later considerations

- An image failure is retried only when another connection attempt occurs. A healthy WebSocket can stay connected indefinitely; this proposal is **not** a guaranteed timed retry.
- Refreshing all applications on each reconnection adds an HTTP request and may re-download images; consider skipping unchanged cached images in a later optimization.
- The Windows `HICON` cache is keyed by local image path, so replacing the contents of an already-decoded icon file may not update its on-screen icon until the cache is invalidated. Missing icons that were never decoded do not have that stale-icon issue.
- Keep network work out of the tray UI thread. Ensure each REST request completes or fails and releases its resources before starting the next WebSocket session.

### Acceptance tests

1. Windows 10: force an application-image download failure on startup, then restore availability and trigger a WebSocket reconnection; verify that the icon appears on a later notification without restarting the client.
2. Confirm normal initial startup and notification display remain unchanged when downloads succeed.
3. Confirm a failed or unavailable `/application` request does not crash the process or enter a tight reconnect loop.
4. Check Windows cross-compilation with MinGW and the existing Linux build.

No implementation is included in this documentation-only change.

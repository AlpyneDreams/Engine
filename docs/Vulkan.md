```cpp
draw()
    device.acquireNextImageKHR()

    device.waitForFences(rpcbFence)
    device.resetFences(rpcbFence)
    dynamic(pscb)
    graphicsQueue.submit(pscb, rpcbFence)

    device.waitForFences(cbFence)
    device.resetFences(cbFence)
    graphicsQueue.submit(cb, cbFence)

    presentQueue.presentKHR()
    
```
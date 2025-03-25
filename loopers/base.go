package loopers

import (
    "fmt"
    "sync"
    "time"
)

type BaseLooper struct {
    Ticker       *time.Ticker
    ExitChan     chan struct{}        // Change to a channel of empty structs
    Interval     time.Duration
    Name         string
    Subprocesses map[string]*BaseLooper // Track subprocesses by name
    mu           sync.Mutex             // Protect access to Subprocesses
}

func (l *BaseLooper) Setup() error {
    fmt.Printf("[%s] Performing setup...\n", l.Name)
    time.Sleep(1 * time.Second)
    fmt.Printf("[%s] Setup complete!\n", l.Name)
    return nil
}

func (l *BaseLooper) Teardown() {
    l.mu.Lock()
    defer l.mu.Unlock()

    fmt.Printf("[%s] Performing teardown...\n", l.Name)
    if l.Ticker != nil {
        l.Ticker.Stop()
    }

    // Gracefully shut down all subprocesses
    for name, process := range l.Subprocesses {
        fmt.Printf("[%s] Stopping subprocess: %s\n", l.Name, name)
        close(process.ExitChan) // Close ExitChan to signal subprocess shutdown
    }
    fmt.Printf("[%s] Teardown complete!\n", l.Name)
}

func (l *BaseLooper) Run() {
    fmt.Printf("[%s] Loop running every %v... Press Ctrl+C to exit.\n", l.Name, l.Interval)
    for {
        select {
        case <-l.Ticker.C:
            fmt.Printf("[%s] Instance doing work...\n", l.Name)
        case <-l.ExitChan:
            fmt.Printf("[%s] Received signal to exit. Exiting gracefully...\n", l.Name)
            l.Teardown()
            return
        }
    }
}

func (l *BaseLooper) AddSubprocess(name string, subprocess interface{}) {
    l.mu.Lock()
    defer l.mu.Unlock()

    // Ensure the subprocess embeds BaseLooper
    switch sp := subprocess.(type) {
    case *BaseLooper:
        l.Subprocesses[name] = sp
    case interface{ GetBaseLooper() *BaseLooper }: // Handle embedded BaseLooper
        l.Subprocesses[name] = sp.GetBaseLooper()
    default:
        panic(fmt.Sprintf("Invalid subprocess type: %T", subprocess))
    }

    go func() {
        switch sp := subprocess.(type) {
        case *NetIntLooper:
            sp.RunNetIntTask() // Call the specialized method
        case *BaseLooper:
            sp.Run() // Default to BaseLooper's Run()
        default:
            panic(fmt.Sprintf("Invalid subprocess type: %T", subprocess))
        }
    }()
}



// Remove a subprocess from the BaseLooper (optional)
func (l *BaseLooper) RemoveSubprocess(name string) {
    l.mu.Lock()
    defer l.mu.Unlock()

    delete(l.Subprocesses, name)
}

func NewLooper(interval time.Duration) *BaseLooper {
    if interval <= 0 {
        interval = 1 * time.Second
    }
    return &BaseLooper{
        Ticker:       time.NewTicker(interval),
        ExitChan:     make(chan struct{}), // Use a struct{} channel
        Interval:     interval,
        Name:         "base",
        Subprocesses: make(map[string]*BaseLooper),
    }
}

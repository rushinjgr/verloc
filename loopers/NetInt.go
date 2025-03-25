package loopers

import (
    "fmt"
    "net"
    "time"
)

type NetIntLooper struct {
    *BaseLooper
}

func (s *NetIntLooper) RunNetIntTask() {
    for {
        select {
        case <-s.Ticker.C:
            fmt.Printf("[%s] Enumerating network interfaces...\n", s.Name)
            interfaces, err := net.Interfaces()
            if err != nil {
                fmt.Printf("[%s] Error retrieving interfaces: %s\n", s.Name, err)
                continue
            }
            for _, iface := range interfaces {
                fmt.Printf("[%s] Interface: %s\n", s.Name, iface.Name)
                addrs, err := iface.Addrs()
                if err != nil {
                    fmt.Printf("[%s] Error retrieving addresses for %s: %s\n", s.Name, iface.Name, err)
                    continue
                }
                for _, addr := range addrs {
                    fmt.Printf("[%s] - Address: %s\n", s.Name, addr.String())
                }
            }
        case <-s.ExitChan:
            fmt.Printf("[%s] NetInt received signal to exit. Cleaning up...\n", s.Name)
            s.Teardown()
            return
        }
    }
}

func NewNetIntLooper(interval time.Duration) *NetIntLooper {
    return &NetIntLooper{
        BaseLooper: &BaseLooper{
            Ticker:       time.NewTicker(interval),
            ExitChan:     make(chan struct{}), // Use a struct{} channel
            Interval:     interval,
            Name:         "NetInt",
            Subprocesses: make(map[string]*BaseLooper),
        },
    }
}

func (s *NetIntLooper) GetBaseLooper() *BaseLooper {
    // TODO move to base
    return s.BaseLooper
}


package loopers

import (
    "fmt"
    "time"
)
type MasterLooper struct {
    *BaseLooper
}

func (m *MasterLooper) Setup() error {
    err := m.BaseLooper.Setup()
    if err != nil {
        return err
    }
    fmt.Printf("[%s] Master setup complete!\n", m.Name)
    return nil
}

func (m *MasterLooper) Teardown() {
    m.BaseLooper.Teardown()
    fmt.Printf("[%s] Master teardown complete!\n", m.Name)
}

func (m *MasterLooper) RunMasterProcess() {
    m.BaseLooper.Run() // Reuse the BaseLooper's Run method
}

func NewMasterLooper(interval time.Duration) *MasterLooper {
    return &MasterLooper{
        BaseLooper: &BaseLooper{
            Ticker:       time.NewTicker(interval),
            ExitChan:     make(chan struct{}), // Updated to chan struct{}
            Interval:     interval,
            Name:         "master",
            Subprocesses: make(map[string]*BaseLooper),
        },
    }
}

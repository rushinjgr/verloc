package main

import (
    "verloc/loopers" // Import the loopers package
    "fmt"
    "time"
)

func main() {
    // Create a MasterLooper
    master := loopers.NewMasterLooper(5 * time.Second)

    // Run MasterLooper setup
    if err := master.Setup(); err != nil {
        fmt.Println("Master setup failed:", err)
        return
    }

    // Create a NetIntLooper and add it as a subprocess
    netInt := loopers.NewNetIntLooper(2 * time.Second)
    master.AddSubprocess("NetInt", netInt)

    // Run the MasterLooper
    master.RunMasterProcess()
}

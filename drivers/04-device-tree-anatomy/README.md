# Device Tree Anatomy: Nodes, Properties, and Hierarchy

In the previous section, we learned *why* we need Device Trees. Now, let's look at **how they are built**. Think of a Device Tree (DT) not just as a configuration file, but as a biological tree where every "branch" represents a piece of hardware.

---

## 1. The "Root" of Everything
Every Device Tree must start with a **Root Node**. In DT syntax, this is represented by a simple forward slash: `/`. 

Just like a file system (where everything starts at `/`), the kernel uses the root node as the starting point to discover all hardware.

### The "Must-Haves"
According to the specification, every root node **must** contain at least:
1.  **`/cpus` node**: Describes the processor cores.
2.  **`/memory` node**: Tells the kernel where the RAM starts and how big it is.

---

## 2. Anatomy of a Device Node
A "node" is just a way to describe a single component (like a UART controller or a sensor). It reveals its data using **Properties**.

```dts
/* Example of a generic node structure */
my_device@12345678 {              // Node Name @ Unit Address
    compatible = "vendor,model";  // Property: Who can drive this?
    reg = <0x12345678 0x100>;     // Property: Where is it in memory?
    status = "okay";              // Property: Is it enabled?
};
```

### Property Types:
*   **Strings:** `model = "Raspberry Pi 5";`
*   **Integers (Cells):** `reg = <0x0 0x100>;` (Inside `< >`)
*   **Boolean:** `mmu-enabled;` (Presence means true, absence means false)
*   **String Lists:** `compatible = "brcm,bcm2712", "arm,cortex-a76";`

---

## 3. Parent-Child Relationships (The I2C Example)
The power of the DT is its hierarchy. Let's look at a realistic scenario: An I2C Controller (the parent) with two sensors (the children) attached to it.

```dts
&i2c0 {                               // The I2C Controller (Parent)
    status = "okay";

    /* Child 1: An Accelerometer */
    accelerometer@68 {                
        compatible = "invensense,mpu6050";
        reg = <0x68>;                 // I2C Address
    };

    /* Child 2: A Temp Sensor (Sibling to the Accelerometer) */
    temp_sensor@48 {
        compatible = "ti,lm75";
        reg = <0x48>;                 // I2C Address
    };
};
```
*   **Parent:** The I2C Controller.
*   **Children:** The MPU6050 and LM75 sensors.
*   **Siblings:** The two sensors are siblings because they share the same parent bus.

---

## 4. Modularity: DTSI vs DTS
In real-world projects like the BeagleBone Black or Raspberry Pi, we don't write everything in one file. We use a modular approach.

### The `.dtsi` (Inclusion File) - "The Blueprint"
The SoC vendor (Broadcom, TI, etc.) provides a `.dtsi` file. It contains the "Global" hardware that never changes (like the CPU cores or the physical addresses of UARTs). 
*   **Rule:** **NEVER edit the `.dtsi` file!** It is shared by many different boards.

### The `.dts` (Source File) - "The Specific Build"
This is *your* file. You include the `.dtsi` and then **override** the parts you need for your specific board.

#### Example: Overriding for your board
Imagine the SoC blueprint (`soc.dtsi`) has a GPU, but it's disabled by default to save power.

```dts
/* In your board_file.dts */
/include/ "soc.dtsi"

&gpu {
    status = "okay";  /* We just 'turned on' the GPU for our board! */
};
```

---

## 5. Key Takeaways
1.  **Nodes** represent devices; **Properties** describe them.
2.  The **Specification** (Chapter 3) is the "Bible" that tells you which properties are allowed.
3.  **Hierarchy** mimics physical connections (Bus -> Device).
4.  **Override, don't edit:** Use labels (like `&i2c0`) in your `.dts` to modify the common logic in the `.dtsi`.

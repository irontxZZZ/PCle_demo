#include <linux/module.h>
#include <linux/pci.h>
#include <linux/io.h>

// 定义虚拟PCI设备ID
#define DEMO_VENDOR_ID 0x1234
// #define DEMO_DEVICE_ID 0xABCD // 用于无硬件模拟
#define DEMO_DEVICE_ID 0x11E8 // 用于QEMU的edu设备

// 定义虚拟设备寄存器偏移
#define REG_FPGA_VERSION    0x00
#define REG_LED_CONTROL     0x04

// 驱动私有数据结构
struct demo_device {
    struct pci_dev *pdev;
    void __iomem *hw_addr; // ioremap后的虚拟基地址
};

// 驱动支持的设备列表
static const struct pci_device_id demo_pci_tbl[] = {
    { PCI_DEVICE(DEMO_VENDOR_ID, DEMO_DEVICE_ID) },
    { 0, } // 列表结束符
};
MODULE_DEVICE_TABLE(pci, demo_pci_tbl);

// 声明probe和remove函数
static int demo_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id);
static void demo_pci_remove(struct pci_dev *pdev);

// PCI驱动结构体
static struct pci_driver demo_pci_driver = {
    .name     = "pci_accelerator",
    .id_table = demo_pci_tbl,
    .probe    = demo_pci_probe,
    .remove   = demo_pci_remove,
};

// ---- 核心Probe函数 ----
static int demo_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct demo_device *dev;
    int ret;
    u32 fpga_version;

    pr_info("PCIeDemo: ==> Probe function CALLED for device %04x:%04x\n", id->vendor, id->device);

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev) {
        return -ENOMEM;
    }
    dev->pdev = pdev;
    pci_set_drvdata(pdev, dev);

    ret = pci_enable_device(pdev);
    if (ret) {
        pr_err("PCIeDemo: Failed to enable PCI device\n");
        return ret;
    }

    ret = pci_request_regions(pdev, "pci_accelerator_regions");
    if (ret) {
        pr_err("PCIeDemo: Failed to request BAR regions\n");
        goto err_disable_device;
    }

    dev->hw_addr = pci_iomap(pdev, 0, 0);
    if (!dev->hw_addr) {
        pr_err("PCIeDemo: Failed to map BAR0\n");
        ret = -EIO;
        goto err_release_regions;
    }
    pr_info("PCIeDemo: BAR0 mapped to kernel virtual address at %p\n", dev->hw_addr);

    pci_set_master(pdev);

    fpga_version = ioread32(dev->hw_addr + REG_FPGA_VERSION);
    pr_info("PCIeDemo: FPGA Version Register [0x%02x] reads: 0x%08x\n", REG_FPGA_VERSION, fpga_version);

    pr_info("PCIeDemo: Turning LED ON by writing '1' to register [0x%02x]\n", REG_LED_CONTROL);
    iowrite32(1, dev->hw_addr + REG_LED_CONTROL);

    pr_info("PCIeDemo: Probe successful!\n");
    return 0;

err_release_regions:
    pci_release_regions(pdev);
err_disable_device:
    pci_disable_device(pdev);
    pr_err("PCIeDemo: Probe failed with error %d\n", ret);
    return ret;
}

// ---- Remove函数 ----
static void demo_pci_remove(struct pci_dev *pdev)
{
    struct demo_device *dev = pci_get_drvdata(pdev);
    pr_info("PCIeDemo: ==> Remove function CALLED.\n");

    if (dev && dev->hw_addr) {
        iowrite32(0, dev->hw_addr + REG_LED_CONTROL);
        pci_iounmap(pdev, dev->hw_addr);
    }
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}

// 内核模块初始化函数
static int __init demo_driver_init(void)
{
    pr_info("PCIeDemo: Loading PCI Accelerator driver...\n");
    return pci_register_driver(&demo_pci_driver);
}

// 内核模块退出函数
static void __exit demo_driver_exit(void)
{
    pr_info("PCIeDemo: Unloading PCI Accelerator driver.\n");
    pci_unregister_driver(&demo_pci_driver);
}

module_init(demo_driver_init);
module_exit(demo_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Demo Author <author@example.com>");
MODULE_DESCRIPTION("A simple PCI driver for demonstration purposes.");

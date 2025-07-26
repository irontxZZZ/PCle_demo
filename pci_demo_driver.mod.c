#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x4a453f53, "iowrite32" },
	{ 0xa1adfcbd, "pci_iounmap" },
	{ 0x1feaa674, "pci_release_regions" },
	{ 0xab219350, "pci_disable_device" },
	{ 0xa728d6a5, "devm_kmalloc" },
	{ 0x57c67651, "pci_enable_device" },
	{ 0xc0d281de, "pci_request_regions" },
	{ 0xeeb97866, "pci_iomap" },
	{ 0xab4e190c, "pci_set_master" },
	{ 0xa78af5f3, "ioread32" },
	{ 0xda5edba0, "pci_unregister_driver" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x9d5676bf, "__pci_register_driver" },
	{ 0xb2b23fc2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00001234d0000ABCDsv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "F0FEC0E2621E6CAE40B639A");

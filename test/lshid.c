/*
 *      lshid.c  --  lsusb like utility for the libhid and USB/HID devices
 *
 *      Copyright (C) 2004
 *        Arnaud Quette <arnaud.quette@mgeups.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 */

#include <hid.h>
#include <stdio.h>
#include <string.h>

char *hid_id[32]; /* FIXME: 32 devices MAX */

/* NOTE: included from libusb/usbi.h. UGLY, i know, but so is libusb! */
struct usb_dev_handle {
  int fd;
  struct usb_bus *bus;
  struct usb_device *device;
  int config;
  int interface;
  int altsetting;
  void *impl_info;
};

bool device_iterator (struct usb_dev_handle const* usbdev, void* custom, unsigned int len)
{
  bool ret = false;
  int i;
  char current_dev_path[10];
 
  /* Obtain the device's full path */
  sprintf(current_dev_path, "%s/%s", usbdev->bus->dirname, usbdev->device->filename);

  /* Check if we already saw this dev */
  for ( i = 0 ; ( hid_id[i] != NULL ) ; i++ )
	{
	  if (!strcmp(hid_id[i], current_dev_path ) )
		break;
	}
  
  /* Append device to the list if needed */
  if (hid_id[i] == NULL)
	{
	  hid_id[i] = (char *) malloc (strlen(usbdev->device->filename) + strlen(usbdev->bus->dirname) );
	  sprintf(hid_id[i], "%s/%s", usbdev->bus->dirname, usbdev->device->filename);
	}
  else /* device already seen */
	{
	  return false;
	}

  /* Filter non HID device */
  if ( (usbdev->device->descriptor.bDeviceClass == 0) /* Class defined at interface level */
	&& usbdev->device->config->interface->altsetting->bInterfaceClass == USB_CLASS_HID)
	  ret = true;
  else
	  ret = false;
  
  return ret;
}

int main(void)
{
  int i;
  hid_return ret;
  HIDInterface* hid;
  HIDInterfaceMatcher matcher;

  /* hid_write_library_config(stdout); */
  
  /* hid_set_debug(HID_DEBUG_NOTRACES); */
  hid_set_debug(HID_DEBUG_NONE);
  hid_set_debug_stream(stderr);
  hid_set_usb_debug(0);

  /* data init */
  for (i = 0 ; i < 32 ; i++)
	hid_id[i] = NULL;

  ret = hid_init();

  hid = hid_new_HIDInterface();
  matcher.vendor_id = HID_ID_MATCH_ANY;
  matcher.product_id = HID_ID_MATCH_ANY;
  matcher.matcher_fn = device_iterator;

  /* open recursively all HID devices found */
  while ( (ret = hid_open(hid, 0, &matcher)) != HID_RET_DEVICE_NOT_FOUND)
	{
	  printf("************************************************************************\n");
	  
	  hid_write_identification(stdout, hid);
	  
	  /* Only dump HID tree if asked */
	  /* hid_dump_tree(stdout, hid); */
	  
	  hid_close(hid);
	}
	 
  hid_delete_HIDInterface(&hid);
  ret = hid_cleanup();
  
  return 0;
}

/* COPYRIGHT --
 *
 * This file is part of libhid, a user-space HID access library.
 * libhid is (c) 2003-2004
 *   Martin F. Krafft <libhid@pobox.madduck.net>
 *   Charles Lepple <clepple@ghz.cc>
 *   Arnaud Quette <arnaud.quette@free.fr> && <arnaud.quette@mgeups.com>
 * and distributed under the terms of the GNU General Public License.
 * See the file ./COPYING in the source distribution for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/// @description Insert description here
// You can write your code in this editor

// Inherit the parent event
event_inherited();

text = "Save (group)";
requestId = noone;

onClick = function() {
	
	var _userId = xboxone_get_savedata_user();
	show_debug_message("[INFO] gdk_save_buffer (userID: " + string(_userId) + ")");

	/* Save a group of buffers. */
	
	var b2 = buffer_create(1, buffer_grow, 1);
	buffer_write(b2, buffer_string, "bar");
	
	var b3 = buffer_create(1, buffer_grow, 1);
	buffer_write(b3, buffer_string, "baz");
	
	var b4 = buffer_create(1, buffer_grow, 1);
	buffer_write(b4, buffer_string, "qux");
	
	switch (os_type) {
		case os_windows:
			windowsSaveGroup(b2, b3, b4);
			break;
		case os_xboxseriesxs:
			xboxSaveGroup(b2, b3, b4)
			break;
			
		default: throw "[ERROR] objSaveGroup, unsupported platform";
	}
	
	buffer_delete(b2);
	buffer_delete(b3);
	buffer_delete(b4);
}

windowsSaveGroup = function(_b2, _b3, _b4) {
	gdk_save_group_begin("multi");
	gdk_save_buffer(_b2, "b2", 0, buffer_get_size(_b2));
	gdk_save_buffer(_b3, "b3", 0, buffer_get_size(_b3));
	gdk_save_buffer(_b4, "b4", 0, buffer_get_size(_b4));
	requestId = gdk_save_group_end();
}

xboxSaveGroup = function(_b2, _b3, _b4) {
	buffer_async_group_begin("multi");
	buffer_save_async(_b2, "b2", 0, buffer_get_size(_b2));
	buffer_save_async(_b3, "b3", 0, buffer_get_size(_b3));
	buffer_save_async(_b4, "b4", 0, buffer_get_size(_b4));
	requestId = buffer_async_group_end();
}


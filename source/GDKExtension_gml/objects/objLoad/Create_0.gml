/// @description Insert description here
// You can write your code in this editor

/// @description Insert description here
// You can write your code in this editor

// Inherit the parent event
event_inherited();

text = "Load (group + single)";
requestIds = {};
errorFlag = false;

requests = [];

onClick = function() {
	
	var _userId = xboxone_get_savedata_user();
	show_debug_message("[INFO] gdk_load_buffer (userID: " + string(_userId) + ")");
	
	errorFlag = false;
	
	// Paths are given without the "root/" prefix, 'queue_load' adds it on GDK.
	queue_load("single/b1");
	queue_load("multi/b2");
	queue_load("multi/b3");
	queue_load("multi/b4");
}

function queue_load(_filename)
{	
	var _bufferId = buffer_create(1, buffer_grow, 1);
	
	switch (os_type) {
		
		case os_windows:
			// On Windows GDK the function used to load buffers asynchronously
			// is 'gdk_load_buffer' it will return a requestID that can be checked
			// during the Async Save/Load event.
			// The Xbox runner nests saves inside a "root" folder, so we prepend
			// "root/" here (and only here) to read back what Xbox wrote.
			var _requestId = gdk_load_buffer(_bufferId, "root/" + _filename, 0, 10);
			break;

		case os_xboxseriesxs:
			// On Xbox Series X/S the function used to load buffers asynchronously
			// is 'buffer_load_async' it will return a requestID that can be checked
			// during the Async Save/Load event.
			var _requestId = buffer_load_async(_bufferId, _filename, 0, 10);
			break;
			
		default: throw "[ERROR] objLoad, unsupported platform";
	}
	
	requestIds[$ _requestId] = { filename: _filename, bufferId: _bufferId };
}



/**
 * @module storage
 * @title Storage Module
 * @desc The Storage Module provides the developer with functions to save and load data associated with a given user.
 * 
 * @section_func Storage Functions
 * @desc The following functions are provided for working with user data storage:
 * @ref gdk_load_buffer
 * @ref gdk_save_buffer
 * @ref gdk_save_group_begin
 * @ref gdk_save_group_end
 * @ref xboxone_get_savedata_user
 * @ref xboxone_set_savedata_user
 * @ref xboxone_set_savedata_uwp_compatibility
 * @section_end
 * 
 * @section Note on cross-platform saves
 * @desc The Xbox runner will always insert saves inside a "root" folder at the top level, so if you need to find those saves with the GDK runner, you need to prepend save locations with "root/", e.g. if you saved "saves/save1.sav" on Xbox, you will find that file at "root/saves/save1.sav" on GDK Windows.
 * @section_end
 * 
 * @module_end
 */

/**
 * @function gdk_load_buffer
 * @desc This function is used to load data from a file that you previously saved using the ${function.gdk_save_buffer} function. The data from the file is loaded into a buffer that you need to have created previously.
 * The `offset` defines the starting position within the buffer for loading (in bytes), and the `size` is the size of the buffer area to be loaded from that offset onwards (also in bytes). You can supply a value of -1 for the `size` argument and the entire buffer will be loaded.
 * Note that the function will load from a "default" folder, which does not need to be included as part of the file path you provide. This folder will be created if it doesn't exist when you save a file using ${function.gdk_save_buffer}.
 * 
 * [[Important: Before using this function it's required to set the workspace for storage operations using the function ${function.xboxone_set_savedata_user}.]]
 * 
 * @param {real} buffer_idx The index of the buffer to load data into
 * @param {string} filename The name of the file to load
 * @param {real} offset The offset within the buffer to load from (in bytes)
 * @param {real} size The size of the buffer area to load (in bytes)
 * 
 * @returns {real} (-1 if there was an error, otherwise the task request ID)
 * 
 * @event save_load
 * @member {real} id The unique identifier of the asynchronous request
 * @member {real} error 0 if successful, some other value if there has been an error (error code)
 * @member {real} status 1 if successful, 0 if failed
 * @member {real} file_size The total size of the file being loaded
 * @member {real} load_size The number of bytes loaded into the buffer
 * @event_end
 * 
 * @example
 * ```gml
 * requestId = gdk_load_buffer(buff, "player_save.sav", 0, 16384);
 * ```
 * In the code above we load a file into a buffer (`buff`). The function call will then return a request ID (`requestId`) that can be used inside an ${event.save_load} event.
 * ```gml
 * if (async_load[? "id"] == requestId)
 *   {
 *       if (async_load[? "status"] == false)
 *       {
 *           show_debug_message("Load failed!");
 *       }
 *       else
 *       {
 *           show_debug_message("Load succeeded!");
 *       }
 *   }
 * ```
 * The code above matches the response against the correct request **id**, providing a success message if **status** is true.
 * @function_end
 */

/**
 * @function gdk_save_buffer
 * @desc With this function you can save a buffer (or a part of it) to a file, ready to be read back into memory using the ${function.gdk_load_buffer} function. The `offset` defines the start position within the buffer to start saving from (in bytes), and the `size` is the size of the buffer area to be saved from that offset onwards (also in bytes).
 * 
 * [[Important: Before using this function it's required to set the workspace for storage operations using the function ${function.xboxone_set_savedata_user}.
 * 
 * @param {real} buffer_idx The index of the buffer to save
 * @param {string} filename The place where to save the buffer to (path + filename + extension)
 * @param {real} offset The start position within the buffer for saving (in bytes)
 * @param {real} size The size of the buffer area to be saved (in bytes)
 * 
 * @returns real (-1 if there was an error, otherwise the task request ID)
 * 
 * @event save_load
 * @member {real} id The unique identifier of the asynchronous request
 * @member {real} error 0 if successful, some other value if there has been an error (error code)
 * @member {real} status 1 if successful, 0 if failed
 * @event_end
 * 
 * @example
 * ```gml
 * requestId = gdk_save_buffer(buff, "player_save.sav", 0, 16384);
 * ```
 * In the code above we save a buffer (`buff`) into a file. The function call will then return a request ID (`requestId`) that can be used inside an ${event.save_load} event.
 * ```gml
 * if (async_load[? "id"] == requestId)
 * {
 *     if (async_load[? "status"] == false)
 *     {
 *         show_debug_message("Save failed!");
 *     }
 *     else
 *     {
 *         show_debug_message("Save succeeded!");
 *     }
 * }
 * ```
 * The code above matches the response against the correct request **id**, providing a success message if **status** is true.
 * @function_end
 */

/**
 * @function gdk_save_group_begin
 * @desc This function is called when you want to begin the saving of multiple buffers to multiple files. The `container_name` is a string and will be used as the directory name for where the files will be saved, and should be used as part of the file path when loading the files back into the IDE later (using the ${function.gdk_load_buffer} function). This function is only for use with the ${function.gdk_save_buffer} function and you must also end the group save by calling ${function.gdk_save_group_end} function, otherwise the files will not be saved out.
 * 
 * @param {string} container_name The name of the container
 * 
 * @example
 * ```gml
 * gdk_save_group_begin("SaveGame");
 * save1 = gdk_save_buffer(buff1, "player_save1.sav", 0, 16384);
 * save2 = gdk_save_buffer(buff2, "player_save2.sav", 0, 16384);
 * save3 = gdk_save_buffer(buff3, "player_save3.sav", 0, 16384);
 * save4 = gdk_save_buffer(buff4, "player_save4.sav", 0, 16384);
 * gdk_save_group_end();
 * ```
 * In the code above we save multiple buffers into different files using a buffer group for them. All the files will be saved inside the same `"SaveGame"` folder.
 * @function_end
 */

/** 
 * @function gdk_save_group_end
 * @desc This function finishes the definition of a buffer save group. You must have previously called the function ${function.gdk_save_group_begin} to initiate the group, then call the function ${function.gdk_save_buffer} for each file that you wish to save out. Finally you call this function, which will start the saving of the files. 
 * 
 * @returns {real} (-1 if there was an error, otherwise the task request ID)
 * 
 * @event save_load
 * @member {real} id The unique identifier of the asynchronous request
 * @member {real} error 0 if successful, some other value if there has been an error (error code)
 * @member {real} status 1 if successful, 0 if failed
 * @event_end
 * 
 * @example
 * ```gml
 * gdk_save_group_begin("SaveGame");
 * gdk_save_buffer(buff1, "player_save1.sav", 0, 16384);
 * gdk_save_buffer(buff2, "player_save2.sav", 0, 16384);
 * gdk_save_buffer(buff3, "player_save3.sav", 0, 16384);
 * gdk_save_buffer(buff4, "player_save4.sav", 0, 16384);
 * requestId = gdk_save_group_end();
 * ```
 * In the code above we save multiple buffers into different files using a buffer group for them. All the files will be saved inside the same `"SaveGame"` folder. Afterwards we end the group with a `gdk_save_group_end()` call, which will then return a request ID (`requestId`) that can be used inside an ${event.save_load}.
 * 
 * ```gml
 * if (async_load[? "id"] == requestId)
 * {
 *     if (async_load[? "status"] == false)
 *     {
 *         show_debug_message("Group save failed!");
 *     }
 *     else
 *     {
 *         show_debug_message("Group save succeeded!");
 *     }
 * }
 * ```
 * The code above matches the response against the correct request **id** , providing a success message if **status** is true.
 * @function_end
 */

/**
 * @function xboxone_get_savedata_user
 * @desc This function returns the user ID pointer (or the constant `pointer_null`) currently associated with file saving. See ${function.xboxone_set_savedata_user} for further details.
 * 
 * @returns {pointer} (The user ID currently being used for save data)
 * 
 * @example
 * ```gml
 * if (xboxone_get_savedata_user() != user_id[0])
 * {
 *     xboxone_set_savedata_user(user_id[0]);
 * }
 * ```
 * In the code above we check to see if the current savedata user is set to the user in the index 0 of the `user_id` array, and if not, we set it to that user id.
 * @function_end
 */

/**
 * @function xboxone_set_savedata_user
 * @desc This function sets all future file operations operating in the save game area (i.e. all file reads and writes made using the ${function.gdk_load_buffer} and ${function.gdk_save_buffer} functions) to be associated with the specified user. This can be called as often as necessary to redirect save data to the appropriate user, or you can use the constant `pointer_null` to lock save/load features.
 * 
 * @param {pointer} user_id The user ID pointer
 * 
 * @example
 * ```gml
 * if (xboxone_get_savedata_user() != user_id[0])
 * {
 *     xboxone_set_savedata_user(user_id[0]);
 * }
 * ```
 * In the code above we check to see if the current savedata user is set to the user in the index 0 of the `user_id` array, and if not, we set it to that user id.
 * @function_end
 */

/**
 * @function xboxone_set_savedata_uwp_compatibility
 * @desc *Added in GameMaker 2023.6*
 * 
 * This function enables compatibility with old UWP save files, as GDK uses a different path format from UWP. Enabling this will make the runner use the same paths as the old UWP runner, making your GDK builds compatible with save files made with the UWP runner.
 * 
 * This should be called before the game performs any savedata related file operations.
 * 
 * This is **only** intended to be used for projects that are updating an already-released UWP title to use GDK instead. It is not meant for use with new projects.
 * 
 * @function_end
 */

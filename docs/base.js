
/**
 * @module base
 * @title Base Module
 * @desc The Base Module in the GDK Extension provides a set of functions to handle the user/account selection for the player.
 * 
 * @section_func Base Functions
 * @desc The following functions are given for working with user accounts:
 * @ref xboxone_show_account_picker
 * @ref xboxone_get_token_and_signature
 * @ref xboxone_get_activating_user
 * @ref xboxone_get_user_count
 * @ref xboxone_get_user
 * @section_end
 * 
 * @module_end
 */


/**
 * @function xboxone_get_activating_user
 * @desc With this function you can retrieve the user ID pointer for the user that launched the game.
 * 
 * [[Note: The returned user ID may not always be accurate as the active account can be changed at any time and this value will always return the user account that was active when the game launched.]]
 * 
 * @returns {pointer} The user ID pointer
 * 
 * @example
 * 
 * ```gml
 * global.main_user = xboxone_get_activating_user();
 * ```
 * In the code above we get the ID of the user responsible for launching the game and store it inside a global variable (`global.main_user`).
 * @function_end
 */

/**
 * @function xboxone_get_token_and_signature
 * @desc This function retrieves xtokens and signatures for web requests.
 * 
 * This function returns -1 if it fails and 0 if succeeds to request the token/signature.
 * 
 * @param {int64} user The user's unique identifier
 * @param {string} url The URL for the web request
 * @param {string} method The method type for the web request (HTTP method)
 * @param {string} json_headers Headers for the web request (JSON-formatted string)
 * @param {string|buffer} body A buffer that contains the body of the web request (also accepts strings)
 * @param {bool} force_refresh Whether a refresh should be forced or not
 * 
 * @returns {real}
 * 
 * @event system
 * @desc 
 * @member {string} event_type The string `"tokenandsignature_result"`
 * @member {real} status Returns `-1` if request failed, `0` if succeeded
 * @member {string} token The token for the provided user
 * @member {string} signature The signature of the HTTP request
 * @event_end
 * 
 * @example
 * ```gml
 * var headers = ds_map_create();
 * headers[?"this"] = "that";
 * headers[?"woot"] = "this";
 * 
 * var body = buffer_create(1, buffer_grow, 1);
 * buffer_write(body, buffer_string, "hello");
 * 
 * var url = "https://profile.xboxlive.com/users/me/profile/settings?settings=GameDisplayName";
 * var request_method = "GET";
 * 
 * if (xboxone_get_token_and_signature(xboxone_get_activating_user(), url, request_method, json_encode(headers), body, true) < 0) {
 *     show_message("Token query failed");
 * }
 * else {
 *     show_debug_message("Requested token.");
 * }
 * 
 * buffer_delete(body);
 * ds_map_destroy(headers);
 * ```
 * In the code above we perform a token request for the activating user with ${function.xboxone_get_activating_user}, this function behaves similarly to to a simple ${function.http_request} in the way you are required to provide a URL, http method (GET, POST, ...), the headers to be used in the request and the body that can be either in buffer or string format. We can catch the function callback inside the ${event.system}, following the code below.
 * 
 * ```gml
 * // Early exit if event type doesn't match
 * if (async_load[?"event_type"] != "tokenandsignature_result") { exit; }
 * 
 * // Validate status
 * var status = async_load[?"status"];
 * if (status == 0) {
 *     // Success
 *     show_message("Token query succeeded. See output.");
 *     show_debug_message("The token of the user that opened the app: " + async_load[?"token"]);
 *     if (ds_map_exists(async_load, "signature")) {
 *         show_debug_message("The signature of the http request: " + async_load[?"signature"]);
 *     }
 * }
 * else {
 *     // Failure
 *     show_message("Token query failed");
 * }
 * ```
 * @function_end
 */

/**
 * @function xboxone_get_user
 * @desc With this function you can retrieve the user ID pointer for the indexed user. If the user does not exist, the function will return the constant `pointer_null` instead. You can find the number of users currently logged in with the function ${function.xboxone_get_user_count}.
 * 
 * @param {real} index The index to get the user ID from
 * 
 * @returns {Pointer} (The user ID pointer)
 * @example
 * ```gml
 * for (var i = 0; i < xboxone_get_user_count(); i++)
 * {
 *     user_id[i] = xboxone_get_user(i);
 * }
 * ```
 * In the code above we loop through all the users currently signed in (using the ${function.xboxone_get_user_count} function) and store their user IDs into an array (`user_id`).
 * @function_end
 */

/**
 * @function xboxone_get_user_count
 * @desc With this function you can find the total number of users currently signed in to the system. The returned value will be an integer.
 * 
 * @returns {real} (The total number of users currently signed in to the system)
 * @example
 * ```gml
 * for (var i = 0; i < xboxone_get_user_count(); i++)
 * {
 *     user_id[i] = xboxone_get_user(i);
 * }
 * ```
 * In the code above we loop through all the users currently signed in to the system and store their user IDs into an array (using the ${function.xboxone_get_user} function).
 * @function_end
 */

/**
 * @function xboxone_show_account_picker
 * @desc This function launches the system's account picker for the specified pad ID, where selecting an account will associate it with the specified pad. The `guests` argument is either `true` or `false` – if `false` is specified no guest accounts can be selected, while `true` allows guest accounts. 
 * 
 * [[Warning: IMPORTANT The argument `pad_id` is ignored by the GDK Extension and is only used when the function is called on console.]]
 * 
 * @param {real} pad_id :warning: UNUSED Pad ID of the user to select the account for
 * @param {boolean} guests Whether we should allow guest accounts
 * 
 * @event dialog
 * @desc 
 * @member {string} type The string `"xboxone_accountpicker"`
 * @member {boolean} succeeded Returns `true` if an account was select, `false` if cancelled
 * @member {pointer} user The store ID used when acquiring the license
 * @event_end
 * 
 * @example
 * ```gml
 * xboxone_show_account_picker(0, false);
 * ```
 * In the code above we first request for an account selection dialog that allows no guest accounts. The function callback can be caught inside an ${event.dialog}, following the code below.
 * 
 * ```gml
 * if (async_load[? "type"] == "xboxone_accountpicker")
 * {
 *     if (async_load[? "succeeded"] == false)
 *     {
 *         show_debug_message("User cancelled account selection!");
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **id**, providing a failure message if `"succeeded"` is `false`.
 * @function_end
 */

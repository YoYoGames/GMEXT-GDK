
/**
 * @module xbox_live
 * @title Xbox Live Module
 * @desc The GDK extension allows your Microsoft Store game to use all features from the Xbox Live services including stats, leaderboards, achievements and rich presence. This module contains all the functions that are available to handle Xbox Live services.
 * 
 * Xbox Live services can either be **Event-Based** or **Title-Managed** and **this choice is made on the Partner Center** (check the [help article on the Partner Center](https://gamemaker.zendesk.com/hc/en-us/articles/4411044955793-Microsoft-Partner-Center-Guide-for-Windows-Store-Xbox-Developers) for more information). The functions below are grouped according to these two systems.
 * 
 * @section_func Event-Based Functions
 * @desc [[Warning: IMPORTANT Using the Event-Based system requires some additional configuration detailed under [Manifest File] guide.
 * The following functions are provided for event-based stats/leaderboards/achievements (Microsoft recommends using these for **stats** and **leaderboards** but not **achievements**):
 * @ref xboxone_stats_setup
 * @ref xboxone_check_privilege
 * @ref xboxone_fire_event
 * @ref xboxone_read_player_leaderboard
 * @section_end
 * 
 * @section_func Title-Managed Functions
 * @desc The following functions are provided for title-managed stats/leaderboards/achievements (Microsoft recommends using these for **achievements**):
 * @ref xboxone_stats_add_user
 * @ref xboxone_stats_remove_user
 * @ref xboxone_stats_flush_user
 * @ref xboxone_stats_get_stat
 * @ref xboxone_stats_get_stat_names
 * @ref xboxone_stats_set_stat_int
 * @ref xboxone_stats_set_stat_real
 * @ref xboxone_stats_set_stat_string
 * @ref xboxone_stats_delete_stat
 * @ref xboxone_stats_get_leaderboard
 * @ref xboxone_stats_get_social_leaderboard
 * @ref xboxone_achievements_set_progress
 * @ref xboxone_get_achievement
 * @section_end
 * 
 * @section_func General Functions
 * @desc The following functions can be used regardless of whether Event-Based or Title-Managed services are used:
 * @ref xboxone_set_rich_presence
 * @ref xboxone_gamertag_for_user
 * @ref xboxone_unique_modern_gamertag_for_user
 * @ref xboxone_modern_gamertag_suffix_for_user
 * @ref xboxone_modern_gamertag_for_user
 * @section_end
 * 
 * @section_const Constants
 * @desc 
 * @ref xboxone_achievement_filter
 * @ref xboxone_achievement_message_type
 * @ref xboxone_privilege
 * @ref xboxone_privilege_result
 * @section_end
 * 
 * @module_end
 */

/**
 * @function xboxone_achievements_set_progress
 * @desc This function can be used to update the progress of an achievement. You supply the `user_id` as returned by the function ${function.xboxone_get_user}, a string containing the `achievement`'s numeric ID (as assigned in the Partner Center when it was created), and finally the `progress` value to set (from 0 to 100).
 * 
 * The function may return one of the following errors:
 * 
 * |Error|Description|
 * |----|----|
 * |-1|Invalid `user_id` was specified.|
 * |-2|Xbox Live context could not be retrieved.|
 * 
 * [[Warning: IMPORTANT Note that the achievement system will refuse updates that are lower than the current progress value.]]
 * 
 * @param {pointer} user_id The user ID pointer
 * @param {string} achievement The achievement identification string (obtained from the Partner Center)
 * @param {real} progress The new progress value of the achievement (is a value between 0 and 100)
 * 
 * @returns {real} (negative if error, otherwise the System Request ID)
 * 
 * @event system
 * @member {string} event_type The string `"achievement result"
 * @member {real} requestID The id of the request that fired this callback
 * @member {string} achievement The achievement ID string passed to the function call
 * @member {real} progress The updated progress value
 * @member {real} error This will be: `0` if the progress update was successful; `xboxone_achievement_already_unlocked` if the achievement was unlocked in a previous request; or a negative number with the error code if request fails
 * @event_end
 * 
 * @example
 * ```gml
 * requestId = xboxone_achievements_set_progress(user_id, "KilledFirstBoss", 100);
 * ```
 * In the code above we're setting the `"KilledFirstBoss"` achievement as 100% complete, the function call will then return a request ID (`requestId`) that can be used inside an ${event.system} event. 
 * ```gml
 * if (async_load[? "event_type"] == "achievement_result")
 * {
 *     if (async_load[? "requestID"] == requestId)
 *     {
 *         if (async_load[? "error"] >= 0)
 *         {
 *             show_debug_message("Request succeeded");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct **event_type** and **requestID** , and prints a debug message if the request was successful.
 * @function_end
 */

/**
 * @function xboxone_get_achievement
 * @desc This function queries Xbox Live for the details of the given achievement, for the user specified.
 * 
 * The function may return one of the following errors:
 * 
 * |Error|Description|
 * |----|----|
 * |-1|Invalid `user_id` was specified.|
 * |-2|Xbox Live context could not be retrieved.|
 * 
 * [[Warning: IMPORTANT Note that the achievement system will refuse updates that are lower than the current progress value.]]
 * 
 * @param {pointer} user_id The user ID pointer
 * @param {string} achievement The achievement identification string (obtained from the Partner Center)
 * 
 * @returns {real} (negative if error, otherwise the System Request ID)
 * 
 * @event system
 * @member {string} event_type The string `"achievement info"`
 * @member {real} requestID The ID of the request that fired this callback.
 * @member {string} achievement The achievement ID string passed to the function call.
 * @member {real} progress The progress value.
 * @member {string} name The UTF-8 encoded localized achievement name.
 * @member {real} progress_state The state of a user's progress towards the earning of the achievement: `0` - unknown; `1` - achieved; `2` - not started; `3` - in progress.
 * @member {int64} userID The user ID pointer.
 * @member {real} error This will be: `0` if the progress update was successful; `xboxone_achievement_already_unlocked` if the achievement was unlocked in a previous request; or a negative number with the error code if the request failed.
 * @event_end
 * 
 * @example
 * ```gml
 * requestId = xboxone_get_achievement(user_id, "KilledFirstBoss");
 * ```
 * In the code above we're setting the `"KilledFirstBoss"` achievement as 100% complete, the function call will then return a request ID (`requestId`) that can be used inside an ${event.system} event. 
 * ```gml
 * if (async_load[? "event_type"] == "achievement info")
 * {
 *     if (async_load[? "requestID"] == requestId)
 *     {
 *         if (async_load[? "error"] >= 0)
 *         {
 *             show_debug_message("Request succeeded");
 *             if (async_load[? "progress_state"] == 3) show_debug_message("We are almost there: " + string(async_load[? "progress"]));
 *             else if (async_load[? "progress_state"] == 1) show_debug_message("We did it!!");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct **event_type** and **requestID**, and prints a debug message if the request was successful.
 * @function_end
 */

/**
 * @function xboxone_check_privilege
 * @desc With this function you can check whether the given user has a privilege. If you set the `attempt_resolution` argument to `true` and the privilege isn't enabled, it will also open a system dialogue (suspending the game) to prompt the user to upgrade their account or to get the privilege in a different way as required. If the user then acquires the required privilege, the function will return `true`.
 * 
 * @param {pointer} user_id The user ID pointer
 * @param {constant.xboxone_privilege} privilege_id The privilege to check for
 * @param {boolean} attempt_resolution Requests for this privilege
 * 
 * @event system
 * @member {string} event_type The string `"check_privilege_result"`
 * @member {constant.xboxone_privilege_result} result One or more (bit-wise combined) of the `xboxone_privilege_result_*` constants
 * @member {constant.xboxone_privilege} privilege The privilege you have requested
 * @event_end
 * 
 * @example
 * ```gml
 * var user_one = xboxone_get_activating_user();
 * xboxone_check_privilege(user_one, xboxone_privilege_multiplayer_sessions, true);
 * ```
 * In the code above we are getting the ID of the user that launched the game (using the function ${function.xboxone_get_activating_user)) and checking if they have privilege for multiplayer sessions and requesting for attempting resolution.
 * @function_end
 */

/**
 * @function xboxone_fire_event
 * @desc This function can be used to fire a stat event. The `event_name` argument is the name of the event to be fired as defined in the Partner Center console for your game, and the following additional parameters will also depend on what you have a set up for the stat.
 * 
 * The function will return 0 if the event was sent (and should be received/processed by the server) or -1 if there was an error (e.g. your events manifest file is outdated).
 * 
 * [[Note: The first two parameters for an event usually default to the `user_id` (obtained from the ${function.xboxone_get_user function) and the game's `scid` (obtained from the Microsoft Partner Center).]]
 * 
 * @param {string} event_name The name of the event to be triggered
 * @param {string} [params] One or more arguments specifying the parameters to be passed to the event as individual arguments
 * 
 * @returns {real} (-1 on error, 0 if the function was successful)
 * 
 * @example
 * ```gml
 * var uid = xboxone_get_activating_user();
 * var result = xboxone_fire_event("PlayerSessionStart", uid, global.scid, 0, 42, 42);
 * ```
 * In the code above we are firing the `"PlayerSessionStart"` event and passing some parameters, including the player's ID and the game's SCID as the first two parameters for the specified event.
 * @function_end
 */

/**
 * @function xboxone_read_player_leaderboard
 * @desc This function allows you to read a leaderboard starting at the specified user, regardless of the user's rank or score, and ordered by percentile rank. You supply the `user_id` as returned by the function ${function.xboxone_get_user} and a `friendfilter` that is one of the ${constant.xboxone_achievement_filter} constants.
 * 
 * [[Warning: IMPORTANT This function requires ${function.xboxone_stats_setup} before it can be used.]]
 * 
 * @param {string} ident The leaderboard id (if filter is `xboxone_achievement_filter_all_players`) or stat to read
 * @param {pointer} user_id The user ID pointer
 * @param {real} numitems The number of items to retrieve
 * @param {constant.xboxone_achievement_filter} friendfilter One of the `xboxone_achievement_filter_*` constants
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_leaderboard_info`
 * @member {string} leaderboardid The unique ID of the leaderboard as defined on the provider dashboard.
 * @member {real} numentries The number of entries in the leaderboard that you have received.
 * @member {string} PlayerN The name of the player, where **N** is position within the received entries list.
 * @member {pointer} PlayeridN The unique user id of the player **N** 
 * @member {real} RankN The rank of the player **N** within the leaderboard.
 * @member {real} ScoreN The score of the player **N** 
 * @event_end
 * 
 * @example
 * 
 * ```gml
 * var uid = xboxone_get_activating_user();
 * xboxone_read_player_leaderboard("MyLeaderboard", uid, 10, xboxone_achievement_filter_all_players);
 * ```
 * In the code above we are querying the leaderboard with the ID `"MyLeaderboard"` for the first 10 entries including all players.
 * We can catch the triggered callback using the ${event.social} event.
 * ```gml
 * if (async_load[? "id"] == xboxone_achievement_leaderboard_info)
 * {
 *     global.numentries = async_load[? "numentries"];
 *     for (var i = 0; i < numentries; i++)
 *     {
 *         global.playername[i] = async_load[? "Player" + string(i)];
 *         global.playerid[i] = async_load[? "Playerid" + string(i)];
 *         global.playerrank[i] = async_load[? "Rank" + string(i)];
 *         global.playerscore[i] = async_load[? "Score" + string(i)];
 *     }
 * }
 * ```
 * The code above matches the response against the correct **id** and stores the information of the top player names, ids, ranks and scores in global arrays.
 * @function_end
 */

/**
 * @function xboxone_set_rich_presence
 * @desc This function is used to set the rich presence string for the given user. A Rich Presence string shows the user's in-game activity after the name of the game that the user is playing, separated by a hyphen. This string is displayed under a player's Gamertag in the "Friends & Clubs" list as well as in the player's Xbox Live profile.
 * When using this function you need to supply the `user_id` for the user, and then you can flag the user as currently active in the game or not (using `true`/`false`). The next argument is the `rich_presence_string` ID to show, and then finally you can (optionally) supply a `scid` string. Note that this is an optional argument -- if you have called ${function.xboxone_stats_setup} you don't need to pass the `scid` here.
 * 
 * [[TIP: For more information on rich presence and how to set up the strings to use in the Partner Center, please see the Microsoft's [Rich Presence](https://docs.microsoft.com/en-us/gaming/xbox-live/features/social/presence/config/live-presence-config2) documentation.]]
 * 
 * [[Warning: IMPORTANT On the Windows platform it is not possible to check your rich presence string during development (when it's a sandboxed project).]]
 * 
 * @param {pointer} user_id The user ID pointer
 * @param {boolean} is_user_active Flag the user as active or not
 * @param {string} rich_presence_id The rich present ID (defined in the Partner Center)
 * @param {string} [scid] :eight_pointed_black_star: OPTIONAL The Service Configuration ID string
 * 
 * @example
 * ```gml
 * var user_one = xboxone_get_activating_user();
 * xboxone_set_rich_presence(userId, true, "Playing_Challenge", global.scid);
 * ```
 * In the code above we are getting the active user (using the function ${function.xboxone_get_activating_user}) and setting its current presence string. We are providing a scid (`global.scid`), otherwise we were required to first call the function ${function.xboxone_stats_setup}.
 * @function_end
 */

/**
 * @function xboxone_gamertag_for_user
 * @desc This function is used to get the gamer tag for the given user. This tag is displayed in a player's "Friends & Clubs" list as well as in the player's Xbox Live profile.
 * 
 * [[Note: For more information on gamer tags, please see the Microsoft's [Modern Gamertag](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/user-profile/gamertags/live-modern-gamertags-overview) documentation.]]
 * 
 * @param {int64} user_id The user unique ID
 * 
 * @returns {string}
 * 
 * @version 3.0.1 (-) Function deprecated
 * 
 * @example
 * ```gml
 * var _user = xboxone_get_activating_user();
 * var _tag = xboxone_gamertag_for_user(_user);
 * show_debug_message(_tag);
 * ```
 * In the code above we are getting the active user (using the function ${function.xboxone_get_activating_user}) and getting its current gamertag.
 * @function_end
 */

/**
 * @function xboxone_unique_modern_gamertag_for_user
 * @desc This function returns the [modern gamertag](https://learn.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/user-profile/gamertags/live-modern-gamertags-overview) (including the suffix) of a locally signed in user or a player in the current game session. If the player does not have a modern gamertag, the classic gamertag is returned instead.
 * 
 * [[Note: For more information on gamer tags, please see the Microsoft's [Modern Gamertag](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/user-profile/gamertags/live-modern-gamertags-overview) documentation.]]
 * 
 * @param {int64} user_id The user unique ID
 * 
 * @returns {string}
 * 
 * @version 3.0.1 (+) Function added
 * 
 * @example
 * ```gml
 * var _user = xboxone_get_activating_user();
 * var _tag = xboxone_unique_modern_gamertag_for_user(_user);
 * show_debug_message(_tag);
 * ```
 * @function_end
 */

/**
 * @function xboxone_modern_gamertag_suffix_for_user
 * @desc This function returns the modern gamertag suffix of a locally signed in user or a player in the current game session (e.g. `"#1234"`). If the player does not have a modern gamertag or the modern gamertag does not include a suffix, an empty string is returned.
 * 
 * [[Note: For more information on gamer tags, please see the Microsoft's [Modern Gamertag](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/user-profile/gamertags/live-modern-gamertags-overview) documentation.]]
 * 
 * @param {int64} user_id The user unique ID
 * 
 * @returns {string}
 * 
 * @version 3.0.1 (+) Function added
 * 
 * @example
 * ```gml
 * var _user = xboxone_get_activating_user();
 * var _tag = xboxone_modern_gamertag_suffix_for_user(_user);
 * show_debug_message(_tag);
 * ```
 * @function_end
 */

/**
 * @function xboxone_modern_gamertag_for_user
 * @desc This function returns the modern gamertag (excluding suffix) of a locally signed in user or a player in the current game session.
 * 
 * [[Note: For more information on gamer tags, please see the Microsoft's [Modern Gamertag](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/live/features/identity/user-profile/gamertags/live-modern-gamertags-overview) documentation.]]
 * 
 * @param {int64} user_id The user unique ID
 * 
 * @returns {string}
 * 
 * @version 3.0.1 (+) Function added
 * 
 * @example
 * ```gml
 * var _user = xboxone_get_activating_user();
 * var _tag = xboxone_modern_gamertag_for_user(_user);
 * show_debug_message(_tag);
 * ```
 * @function_end
 */

/**
 * @function xboxone_stats_setup
 * @desc This function needs to be called before you can use any of the other Xbox stat functions, and simply initializes the required libraries on the system. The `user_id` argument is the user ID as returned by the function ${function.xboxone_get_user}, while the `scid` and `title_id` are the unique ID's for your game on the Microsoft Partner Center.
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} scid The Service Configuration ID (SCID)
 * @param {real} title_id (hex)|The title ID (as shown in `"MicrosoftGame.Config"`)
 * 
 * @example
 * ```gml
 * var user_id = xboxone_get_user(0);
 * xboxone_stats_setup(user_id, "00000000-0000-0000-0000-000000000000", 0xFFFFFFFF);
 * ```
 * In the code above first we retrieve the user ID of the user with index 0 (using the ${function.xboxone_get_user} function) and use it to set up the event-based stat system.
 * @function_end
 */

/**
 * @function xboxone_stats_add_user
 * @desc This function can be used to add the given user to the statistics manager. This must be done before using any of the other stats functions to automatically sync the game with the Xbox Live server and retrieve the latest values. You supply the `user_id` as returned by the function ${function.xboxone_get_user}, and the function will return -1 if there was an error or the `user_id` is invalid, or an async request ID if the function was successfully called.
 * 
 * @param {pointer} user_id The user ID pointer
 * 
 * @returns {real} (-1 on error, otherwise an async request ID)
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_stat_event`
 * @member {string} event The string `"LocalUserAdded"`.
 * @member {pointer} userid The user ID associated with the request.
 * @member {real} error 0 if successful, some other value on error.
 * @member {string} [errorMessage] :eight_pointed_black_star: OPTIONAL A string with an error message
 * @event_end
 * 
 * @example
 * ```gml
 * for(var i = 0; i < xboxone_get_user_count(); i++)
 * {
 *     user_id[i] = xboxone_get_user(i);
 *     xboxone_stats_add_user(user_id[i]);
 * }
 * ```
 * In the code above we are looping through all the available users (using the function ${function.xboxone_get_user_count}) getting their user id (using the ${function.xboxone_get_user} function) and adding them to the statistics manager.
 * We can catch the triggered callbacks using the ${event.social} event.
 * 
 * ```gml
 * if (async_load[? "event"] == "LocalUserAdded")
 * {
 *     if (async_load[? "error"] != 0)
 *     {
 *         show_debug_message("Error while adding user to statistics manager");
 *     }
 * }
 * ```
 * The code above matches the response against the correct **event** , providing a failure message if **error** is not 0.
 * @function_end
 */

/**
 * @function xboxone_stats_delete_stat
 * @desc This function can be used to delete a stat from the stat manager for the given user ID. You supply the user ID as returned by the function ${function.xboxone_get_user}, then the stat string to delete. This clears the stat value and removes it from the stat manager, meaning it will no longer be returned by the functions ${function.xboxone_stats_get_stat_names} and ${function.xboxone_stats_get_stat}.
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat_name The stat to be deleted.
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @example
 * ```gml
 * for (var i = 0; i < xboxone_get_user_count(); i++)
 * {
 *     user_id[i] = xboxone_get_user(i);
 *     xboxone_stats_delete_stat(user_id[i], "highScore");
 * }
 * ```
 * In the code above we are looping though all the available users (using the function ${function.xboxone_get_user_count}) getting their user id (using the ${function.xboxone_get_user} function) and deleting the stat `highScore` from each one of them.
 * @function_end
 */

/**
 * @function xboxone_stats_flush_user
 * @desc This function can be used to flush the stats data for the given user from the statistics manager to the live server, ensuring that the server is up to date with the current values.  To use the function, you supply the `user_id` as returned by the function ${function.xboxone_get_user}, and then give a priority value (0 for low priority and 1 for high priority). The function will will return -1 if there was an error or the user ID was invalid, or an async request ID if the function was successfully called.
 * 
 * [[Warning: IMPORTANT According to Xbox documentation, developers should be careful not to call this too often as the Xbox system will rate-limit the requests, and the GDK Extension will also automatically flush stats approximately every 5 minutes.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {boolean} high_priority Whether or not flush is high priority.
 * 
 * @returns {real} (-1 on error, otherwise an async request ID)
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_stat_event`
 * @member {string} event The string `"StatisticUpdateComplete"`.
 * @member {pointer} userid The user ID associated with the request.
 * @member {real} error 0 if successful, some other value on error.
 * @member {string} [errorMessage] A string with an error message
 * @event_end
 * 
 * @example
 * ```gml
 * for(var i = 0; i < array_length(user_ids); i++)
 * {
 *     xboxone_stats_flush_user(user_ids[i]);
 * }
 * ```
 * In the code above we are looping though an array of user ids (`user_ids`) and flushing their local data to the live server.
 * We can catch the triggered callbacks using the ${event.social}. 
 * 
 * ```gml
 * if (async_load[? "event"] == "StatisticsUpdateComplete")
 * {
 *     if (async_load[? "error"] != 0)
 *     {
 *         show_debug_message("Error while updating user statistics");
 *     }
 * }
 * ```
 * The code above matches the response against the correct **event** , providing a failure message if **error** is not 0.
 * @function_end
 */

/**
 * @function xboxone_stats_get_leaderboard
 * @desc This function can be used to retrieve a global leaderboard with ranks for a given statistic. You supply the user ID (as returned by the function ${function.xboxone_get_user}), the stat string (as defined when you registered it as a "Featured Stat"), and then you specify a number of details about what leaderboard information you want to retrieve. Note that you can only retrieve a global leaderboard for `int` or `real` stats, but not for `string` stats.
 * 
 * [[Warning: IMPORTANT Stats used in global leaderboards must be registered as "Featured Stats" in the Partner Center otherwise an error will be returned. If you want local (social) leaderboards, then please see the function ${function.xboxone_stats_get_social_leaderboard}.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat The stat to create the global leaderboard from.
 * @param {real} num_entries The number of entries from the leaderboard to retrieve.
 * @param {real} start_rank The rank in the leaderboard to start from (set to `0` if `start_at_user` is set to true).
 * @param {boolean} start_at_user Set to `true` to start at the user ID.
 * @param {boolean} ascending Set to `true` or ascending or `false` for descending order.
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_leaderboard_info`
 * @member {string} event The string `"GetLeaderboardComplete"`.
 * @member {pointer} userid The user ID associated with the request.
 * @member {real} error 0 if successful, some other value on error.
 * @member {string} errorMessage A string with an error message **[OPTIONAL]** 
 * @member {string} displayName The unique ID for the leaderboard as defined on the provider dashboard.
 * @member {real} numentries The number of entries in the leaderboard that you have received.
 * @member {string} PlayerN The name of the player, where **N** is position within the received entries list.
 * @member {pointer} PlayeridN The unique user id of the player **N** 
 * @member {real} RankN The rank of the player **N** within the leaderboard.
 * @member {real} ScoreN The score of the player **N**
 * @event_end
 * 
 * @example
 * ```gml
 * xboxone_stats_get_leaderboard(user_id, "GlobalTime", 20, 1, false, true);
 * ```
 * In the code above we are querying a leaderboard for the stat `"GlobalTime"` with the first 20 entries starting on rank 1 in ascending order.
 * We can catch the triggered callback using the ${event.social} event. 
 * ```gml
 * if (async_load[? "id"] == achievement_leaderboard_info)
 * {
 *     if (async_load[? "event"] == "GetLeaderboardComplete")
 *     {
 *         global.numentries = async_load[? "numentries"];
 *         for (var i = 0; i < numentries; i++)
 *         {
 *             global.playername[i] = async_load[? "Player" + string(i)];
 *             global.playerid[i] = async_load[? "Playerid" + string(i)];
 *             global.playerrank[i] = async_load[? "Rank" + string(i)];
 *             global.playerscore[i] = async_load[? "Score" + string(i)];
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct **id** and **event** and stores the information of the top player names, ids, ranks and scores in global arrays.
 * @function_end
 */

/**
 * @function xboxone_stats_get_social_leaderboard
 * @desc This function can be used to retrieve a social leaderboard with ranks for a given statistic. You supply the user ID (as returned by the function ${function.xboxone_get_user}), the stat string (as defined when you created it using the `xboxone_stats_set_stat_*` functions), and then you specify a number of details about what leaderboard information you want to retrieve.
 * 
 * Note that you can only retrieve a social leaderboard for int or real stats, but not for string stats, and that if you flag the `favourites_only` argument as `true`, then the results will only contain data for those friends that are marked by the user as "favorites".
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * [[Tip: Stats used in social leaderboards do not need to be registered as "Featured Stats" in the Partner Center.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat The stat to create the global leaderboard from.
 * @param {real} num_entries The number of entries from the leaderboard to retrieve.
 * @param {real} start_rank The rank in the leaderboard to start from (set to `0` if `start_at_user` is set to true).
 * @param {boolean} start_at_user Set to `true` to start at the user ID.
 * @param {boolean} ascending Set to `true` or ascending or `false` for descending order.
 * @param {boolean} favourites_only Set to `true` to show only friends that are marked as favourites.
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_leaderboard_info`
 * @member {string} event The string `"GetLeaderboardComplete"`.
 * @member {pointer} userid The user ID associated with the request.
 * @member {real} error 0 if successful, some other value on error.
 * @member {string} errorMessage A string with an error message **[OPTIONAL]** 
 * @member {string} displayName The unique ID for the leaderboard as defined on the provider dashboard.
 * @member {real} numentries The number of entries in the leaderboard that you have received.
 * @member {string} PlayerN The name of the player, where **N** is position within the received entries list.
 * @member {pointer} PlayeridN The unique user id of the player **N** 
 * @member {real} RankN The rank of the player **N** within the leaderboard.
 * @member {real} ScoreN The score of the player **N**
 * @event_end
 * 
 * @example
 * ```gml
 * xboxone_stats_get_social_leaderboard(user_id, "GlobalTime", 20, 1, false, true, true);
 * ```
 * In the code above we are querying a leaderboard for the stat `"GlobalTime"` with the first 20 entries starting on rank 1 in ascending order, selecting only friends marked as favourites.
 * We can catch the triggered callback using the ${event.social}.
 * ```gml
 * if (async_load[? "id"] == achievement_leaderboard_info)
 * {
 *     if (async_load[? "event"] == "GetLeaderboardComplete")
 *     {
 *         global.numentries = async_load[? "numentries"];
 *         for (var i = 0; i < numentries; i++)
 *         {
 *             global.playername[i] = async_load[? "Player" + string(i)];
 *             global.playerid[i] = async_load[? "Playerid" + string(i)];
 *             global.playerrank[i] = async_load[? "Rank" + string(i)];
 *             global.playerscore[i] = async_load[? "Score" + string(i)];
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct id and event and store the information of the top favourite friends player names, ids, ranks and scores in global arrays.
 * @function_end
 */


/**
 * @function xboxone_stats_get_stat
 * @desc This function can be used to retrieve a single stat value from the stat manager for the given user. You supply the `user_id` as returned by the function ${function.xboxone_get_user}, and then the `stat_name` as defined when you created it using one of the `xboxone_stats_set_stat_*` functions. The return value can be either a string or a real (depending on the stat being checked) or the GML constant `undefined` if the stat does not exist or there has been an error.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat_name The statistic to get.
 * 
 * @returns {real|string} (The value for the given stat) or undefined
 * 
 * @example
 * ```gml
 * if (game_over == true)
 * {
 *     if (xboxone_stats_get_stat(user_id, "PercentDone") < 100)
 *     {
 *         var _val = (global.LevelsFinished / global.LevelsTotal)*100;
 *         xboxone_stats_set_stat_real(user_id, "PercentDone", _val);
 *     }
 * }
 * ```
 * In the code above we are checking if the given user complete percentage (`"PercentDone"`) is less then 100 and if so updated it to a new value (using the ${function.xboxone_stats_set_stat_real} function).
 * @function_end
 */

/**
 * @function xboxone_stats_get_stat_names
 * @desc This function can be used to retrieve all the defined stats from the stat manager for the given user. You supply the user ID as returned by the function ${function.xboxone_get_user}, and the function returns an array of strings containing the statistics for the user. If an error occurs or the user has no stats, an empty array will be returned.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * 
 * @returns {array[string]} Array with all the stat names
 * 
 * @example
 * ```gml
 * var _stats = xboxone_stats_get_stat_names(user_id);
 * for (var i = 0; i < array_length(_stats); i++)
 * {
 *     xboxone_stats_delete_stat(user_id, _stats[i]);
 * }
 * ```
 * In the code above we are getting an array of stat names (`_stats`) for a given user and looping through it deleting each stat (using the ${function.xboxone_stats_delete_stat} function).
 * @function_end
 */

/**
 * @function xboxone_stats_remove_user
 * @desc This function can be used to remove (unregister) the given user from the statistics manager, performing a flush of the stat data to the live server. According to the Xbox documentation the game does not have to remove the user from the stats manager, as the GDK Extension will periodically flush the stats anyway.
 * 
 * To use the function, you supply the `user_id` as returned by the function ${function.xboxone_get_user}, and the function will return -1 if there was an error or the user ID was invalid, or any other value if the function was successfully called.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.
 * 
 * [[Warning: IMPORTANT Removing the user can return an error if the statistics that have been set on the user are invalid (such as the stat names containing non-alphanumeric characters).]]
 * 
 * [[Tip: If you want to flush the stats data to the live server at any time without removing the user, you can use the function ${function.xboxone_stats_flush_user}.
 * 
 * @param {pointer} user_id The user ID pointer.
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @event social
 * @member {constant.xboxone_achievement_message_type} id The constant `achievement_stat_event`
 * @member {string} event The string `"LocalUserRemoved"`.
 * @member {pointer} userid The user ID associated with the request.
 * @member {real} error 0 if successful, some other value on error.
 * @member {string} [errorMessage] A string with an error message
 * @event_end
 * 
 * @example
 * ```gml
 * for(var i = 0; i < array_length(user_ids); i++)
 * {
 *     xboxone_stats_remove_user(user_ids[i]);
 * }
 * ```
 * In the code above we are looping through an array of user ids (`user_ids`) and removing them from the statistics manager.
 * We can catch the triggered callbacks using the ${event.social}. 
 * ```gml
 * if (async_load[? "event"] == "LocalUserRemoved")
 * {
 *     if (async_load[? "error"] != 0)
 *     {
 *         show_debug_message("Error while removing user from statistics manager");
 *     }
 * }
 * ```
 * The code above matches the response against the correct **event** , providing a failure message if **error** is not 0.
 * @function_end
 */

/**
 * @function xboxone_stats_set_stat_int
 * @desc This function can be used to set the value of a stat for the given user ID. You supply the user ID as returned by the function ${function.xboxone_get_user}, then the stat string to set (if the stat string does not already exist then a new stat will be created and set to the given value) and a value (an **integer** ) to set the stat to. Note that the stat name must be alphanumeric only, with no symbols or spaces.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * [[Tip: When setting a stat value, any previous values will be overwritten, therefore it is up to you to determine if the stat value should be updated or not (i.e. check that the high score is actually the highest) by comparing to the current stat value with the new one before setting it.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat_name The statistic to set.
 * @param {real} stat_value The value to set the stat to.
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @example
 * ```gml
 * var _highscore = 123;
 * xboxone_stats_set_stat_int(users_ids[0], "Highscore", _highscore);
 * ```
 * In the code above we are setting the stat `"Highscore"` of the selected user (`user_ids[0]`) to a new value, overwriting any previous recorded value.
 * @function_end
 */

/**
 * @function xboxone_stats_set_stat_real
 * @desc This function can be used to set the value of a stat for the given user ID. You supply the user ID as returned by the function ${function.xboxone_get_user}, then the stat string to set (if the stat string does not already exist then a new stat will be created and set to the given value) and a value (a **real**) to set the stat to. Note that the stat name must be alphanumeric only, with no symbols or spaces.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * [[Tip: When setting the stat value, any previous values will be overwritten, therefore it is up to you to determine if the stat value should be updated or not (i.e. check that the high score is actually the highest) by comparing to the current stat value with the new one before setting it.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat_name The statistic to set.
 * @param {real} stat_value The value to set the stat to.
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @example
 * ```gml
 * var _bestTime = 123.45;
 * xboxone_stats_set_stat_real(users_ids[0], "BestTime", _bestTime);
 * ```
 * In the code above we are setting the stat `"BestTime"` of the selected user (`user_ids[0]`) to a new value, overwriting any previous recorded value.
 * @function_end
 */

/**
 * @function xboxone_stats_set_stat_string
 * @desc This function can be used to set the value of a stat for the given user ID. You supply the user ID as returned by the function [xboxone_get_user](Base-Module#xboxone_get_user), then the stat string to set (if the stat string does not already exist then a new stat will be created and set to the given value) and a value (a **string**) to set the stat to. Note that the stat name must be alphanumeric only, with no symbols or spaces.
 * 
 * [[Warning: IMPORTANT The user ID passed into this function should first be added to the statistics manager using the ${function.xboxone_stats_add_user} function.]]
 * 
 * [[Tip: When setting the stat value, any previous values will be overwritten, therefore it is up to you to determine if the stat value should be updated or not (i.e. check that the high score is actually the highest) by comparing to the current stat value with the new one before setting it.]]
 * 
 * @param {pointer} user_id The user ID pointer.
 * @param {string} stat_name The statistic to set.
 * @param {string} stat_value The value to set the stat to.
 * 
 * @returns {real} (-1 on error, any other value otherwise)
 * 
 * @example
 * ```gml
 * xboxone_stats_set_stat_string(users_ids[0], "Team", "YoYo Games");
 * ```
 * In the code above we are setting the stat `"Team"` of the selected user (`user_ids[0]`) to a new value, overwriting any previous recorded value.
 * @function_end
 */

/**
 * @constant xboxone_achievement_filter
 * @desc 
 * @member xboxone_achievement_filter_all_players 
 * @member xboxone_achievement_filter_friends_only 
 * @member xboxone_achievement_filter_favorites_only 
 * @member xboxone_achievement_filter_friends_alt 
 * @member xboxone_achievement_filter_favorites_alt 
 * @constant_end
 */

/**
 * @constant xboxone_achievement_message_type
 * @desc 
 * @member xboxone_achievement_our_info 
 * @member xboxone_achievement_friends_info 
 * @member xboxone_achievement_leaderboard_info 
 * @member xboxone_achievement_achievement_info 
 * @member xboxone_achievement_pic_loaded 
 * @member xboxone_achievement_challenge_completed 
 * @member xboxone_achievement_challenge_completed_by_remote 
 * @member xboxone_achievement_challenge_received 
 * @member xboxone_achievement_challenge_list_received 
 * @member xboxone_achievement_challenge_launched 
 * @member xboxone_achievement_player_info 
 * @member xboxone_achievement_purchase_info 
 * @member xboxone_achievement_msg_result 
 * @member xboxone_achievement_stat_even 
 * @constant_end
 */

/**
 * @constant xboxone_privilege
 * @desc 
 * @member xboxone_privilege_internet_browsing 
 * @member xboxone_privilege_social_network_sharing 
 * @member xboxone_privilege_share_kinect_content 
 * @member xboxone_privilege_video_communications 
 * @member xboxone_privilege_communications 
 * @member xboxone_privilege_user_created_content 
 * @member xboxone_privilege_multiplayer_sessions 
 * @member xboxone_privilege_sessions 
 * @member xboxone_privilege_fitness_upload 
 * @constant_end
 */

/**
 * @constant xboxone_privilege_result
 * @desc 
 * @member xboxone_privilege_result_aborted The check was aborted
 * @member xboxone_privilege_result_banned The user is banned
 * @member xboxone_privilege_result_no_issue There are no privilege issues with the user
 * @member xboxone_privilege_result_purchase_required The user must purchase something additional, usually a subscription, for access
 * @member xboxone_privilege_result_restricted The user is restricted from access, usually through parental controls
 * @member xboxone_privilege_result_unknown Unknown result
 * @constant_end
 */

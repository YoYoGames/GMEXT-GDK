
/**
 * @function gdk_init
 * @desc This function must be called before any other GDK extension function. It is recommended to use a persistent controller/manager object that is created or placed in the first room and to then call this function in its ${event.create}.
 * 
 * @param {string} scid The service configuration ID (found under **Game setup** on the Partner Center).
 * 
 * @example
 * ```gml
 * gdk_init("00000000-0000-0000-0000-000060ddb039");
 * ```
 * The code above initialises the GDK Extension with the provided **SCID** string. This value can be acquired from your `MicrosoftGame.Config` file (that needs to be placed in the included files) or from the Partner Center.
 * @function_end
 */

/**
 * @function gdk_update
 * @desc This should be called each frame while GDK extension is active, recommend using a Controller persistent object that is created or placed in the first room and this call is in the ${event.step}.
 * 
 * 
 * @example
 * ```gml
 * gdk_update();
 * ```
 * In the code above we are ticking the update logic of the GDK extension, faling to call this function will make the extension stop working.
 * @function_end
 */

/**
 * @function gdk_quit
 * @desc This should be called on close down and no GDK extension functions should be called after it, recommend using a Controller (persistent) object that is created or placed in the first room and this call is in its ${event.destroy}.
 * 
 * @example
 * ```gml
 * gdk_quit();
 * ```
 * In the code above will terminate the GDK extension and no GDK extension functions should be called after it.
 * @function_end
 */
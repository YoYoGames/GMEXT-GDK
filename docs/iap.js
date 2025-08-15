/**
 * @module iap
 * @title In-App Purchases Module
 * @desc A Microsoft Store game can have several In-App Purchases. Inside the Partner Center these are referred to as **add-ons** and can be of multiple types (consumables, durables, subscriptions, DLCs). For detailed instructions on how to configure those, check the Add-ons section on the [Microsoft Partner Center](https://help.yoyogames.com/hc/en-us/articles/4411044955793) Helpdesk page.
 * 
 * Contents of this module can be divided into the following sections:
 * 
 * @section_func Product Functions
 * @desc The following functions are provided for working with products:
 * @ref ms_iap_AcquireLicenseForDurables
 * @ref ms_iap_ReleaseLicenseForDurables
 * @ref ms_iap_CanAcquireLicenseForStoreId
 * @ref ms_iap_QueryAddOnLicenses
 * @ref ms_iap_QueryAssociatedProducts
 * @ref ms_iap_QueryConsumableBalanceRemaining
 * @ref ms_iap_QueryEntitledProducts
 * @ref ms_iap_QueryGameLicense
 * @ref ms_iap_QueryProductForCurrentGame
 * @ref ms_iap_QueryProductForPackage
 * @ref ms_iap_QueryProducts
 * @ref ms_iap_ReportConsumableFulfillment
 * @ref ms_iap_ShowAssociatedProductsUI
 * @ref ms_iap_ShowProductPageUI
 * @ref ms_iap_ShowPurchaseUI
 * @ref ms_iap_ShowRateAndReviewUI
 * @ref ms_iap_ShowRedeemTokenUI
 * @section_end
 * 
 * @section_func Package Functions
 * @desc The following functions are provided for working with packages (DLCs):
 * @ref ms_iap_AcquireLicenseForPackage
 * @ref ms_iap_ReleaseLicenseForPackage
 * @ref ms_iap_CanAcquireLicenseForPackage
 * @ref ms_iap_DownloadAndInstallPackages
 * @ref ms_iap_EnumeratePackages
 * @ref ms_iap_MountPackage
 * @ref ms_iap_UnmountPackage
 * @section_end
 * 
 * @section_const Constants
 * @desc The following constants (enumerations) are used as filter arguments and return values for some API queries:
 * @ref PackageKind
 * @ref PackageScope
 * @ref ProductKind
 * @section_end
 * 
 * @section_struct Structs
 * @desc Some of the API asynchronous callback responses return data in the form of structs. This sections aims to deliver detailed information on each of the structs used within the In-App Purchases Module context.
 * The following **structs** (structures) are used as return members of API function calls:
 * @ref AddonLicenseDetails
 * @ref PackageDetails
 * @ref ProductDetails
 * @ref Price
 * @section_end
 * @module_end
 */

/**
 * @function ms_iap_AcquireLicenseForDurables
 * @desc This function acquires a license for the current game's specified durable that the user is entitled to. This is only applicable to  **Durable without package**  add-on types. For  **Durable with packages** (DLC) type, use ${function.ms_iap_AcquireLicenseForPackage} instead.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The store ID of the product.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_AcquireLicenseForDurables_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {string} store_id The store ID used when acquiring the license.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * 
 * requestId = ms_iap_AcquireLicenseForDurables(_userId, _storeId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we get the store ID (`_storeId`) of the product we want to acquire the license to (note that for convenience we keep the products stored in a global variable). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_AcquireLicenseForDurables_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message("License successfully acquired!");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id** , providing a success message if **status** is true.
 * @function_end
 */

/**
 * @function ms_iap_AcquireLicenseForPackage
 * @desc Acquires a license for the current game's specified DLC that the user is entitled to use. This is only applicable to  **Durable with package**  add-on type. For  **Durable**  type, use ${function.ms_iap_AcquireLicenseForDurables} instead.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} package_id A string that uniquely identifies a store package.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_AcquireLicenseForPackage_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {string} package_id The package ID used when acquiring the license.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _packageId = global.packages[0].packageId;
 * 
 * requestId = ms_iap_AcquireLicenseForPackage(_userId, _storeId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we get the package ID (`_packageId`) of the product we want to acquire the license to (note that for convenience we keep the products stored in a global variable). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_AcquireLicenseForPackage_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message("License for package successfully acquired!");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id** , providing a success message if **status** is true.
 * @function_end
 */

/**
 * @function ms_iap_CanAcquireLicenseForPackage
 * @desc This function provides the ability for a game to determine if the user could acquire a license for a particular piece of content without actually acquiring that license and using up that user's concurrency slot. This cannot be used for the current base game (since it has a license already), but to determine if the user owns some DLC for that game.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} package_id A string that uniquely identifies a store package.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The string value `"ms_iap_CanAcquireLicenseForPackage_result"`.
 * @member {real} async_id The asynchronous request ID.
 * @member {bool} async_status Whether or not the asynchronous request succeeded.
 * @member {string} package_id A string that uniquely identifies a store package.
 * @member {string} licensableSku The SKU the user would be able to license.
 * @member {real} licenseStatus Indicates if a user would be able to license a package:<br> 0 - The package is not licensable to the user.<br> 1 - The package is licensable to the user.<br> 2 - The product is not individually licensable.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * 
 * requestId = ms_iap_AcquireLicenseForDurables(_userId, _storeId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we get the store ID (`_storeId`) of the product we want to acquire the license to (note that for convenience we keep the products stored in a global variable). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_CanAcquireLicenseForPackage_result")
 * {
 *     if (async_load[? "async_id"] == requestId)
 *     {
 *         if (async_load[? "licensableStatus"] == 1)
 *         {
 *             show_debug_message("The product is licensable!");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **async_id** , printing to the debug console if the current product is licensable to the provided user.
 * @function_end
 */

/**
 * @function ms_iap_CanAcquireLicenseForStoreId
 * @desc This function provides the ability for a game to determine if the user could acquire a license for a particular piece of content without actually acquiring that license and using up that user's concurrency slot. This is not used for the currently running game (since it has a license already), but to determine if the user owns other games from the same publisher, either to up-sell or to provide special benefits to loyal fans.
 * 
 * For example: this could be used by a racing game to determine whether to give bonus cars to users who owned prior versions of the game. This function is also used to determine whether or not the game should show content in its in game store based on whether or not the user already owns some content.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The store ID of the product to check the license status.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @member {string} type The string value `"ms_iap_CanAcquireLicenseForStoreId_result"`.
 * @member {real} async_id The asynchronous request ID.
 * @member {bool} async_status Whether or not the asynchronous request succeeded.
 * @member {string} store_id The store ID of the product being checked.
 * @member {string} licensableSku The SKU the user would be able to license.
 * @member {real} licenseStatus Indicates if a user would be able to license a package:<br> 0 - The package is not licensable to the user.<br> 1 - The package is licensable to the user.<br> 2 - The product is not individually licensable.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * 
 * requestId = ms_iap_AcquireLicenseForDurables(_userId, _storeId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we get the store ID (`_storeId`) of the product we want to acquire the license to (note that for convenience we keep the products stored in a global variable). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_CanAcquireLicenseForStoreId_result")
 * {
 *     if (async_load[? "async_id"] == requestId)
 *     {
 *         if (async_load[? "licensableStatus"] == 1)
 *         {
 *             show_debug_message("The product is licensable!");
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **async_id** , printing to the debug console if the current product is licensable to the provided user.
 * @function_end
 */

/**
 * @function ms_iap_DownloadAndInstallPackages
 * @desc Downloads and installs the specified store packages.
 * 
 * The async completion message will only be raised when the package is actually installed (not just registered).
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {array[string]} package_ids An array of strings that uniquely identify the store packages.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The string value `"ms_iap_DownloadAndInstallPackages_result"`.
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[string]} package_ids An array of strings that uniquely identify the store packages.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _package1 = global.package[0].packageId;
 * var _package2 = global.package[1].packageId;
 * var _package2 = global.package[2].packageId;
 * 
 * requestId = ms_iap_AcquireLicenseForDurables(_userId, [_package1, _package2, _package3]);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and after that we execute the function passing in an array of packages to be downloaded and installed. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_DownloadAndInstallPackages_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"] == 1)
 *         {
 *             show_debug_message(async_load[? "package_ids"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console an array with all the installed package IDs.
 * @function_end
 */

/**
 * @function ms_iap_EnumeratePackages
 * @desc This function enumerates the results of a package query.
 * 
 * @param {constant.PackageKind} package_kind The value that indicates whether to enumerate app packages or content packages
 * @param {constant.PackageScope} scope The scope of the installation packages
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The string value `"ms_iap_EnumeratePackages_result"`.
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.PackageDetails]} results An array of Package Details structs.
 * @event_end
 * 
 * @example
 * ```gml
 * requestId = ms_iap_EnumeratePackages(e_ms_iap_PackageKind_Content, e_ms_iap_PackageEnumerationScope_ThisOnly);
 * ```
 * In the code above first we request an enumeration of all the packages of type content (`e_ms_iap_PackageKind_Content`) and whose scope is limited to the current game (`e_ms_iap_PackageEnumerationScope_ThisOnly`). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_EnumeratePackages_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"] == 1)
 *         {
 *             show_debug_message(async_load[? "results"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console an array with all the ${struct.PackageDetails} structs.
 * @function_end
 */


/**
 * @function ms_iap_MountPackage
 * @desc This function mounts the installation of specified content.
 * 
 * @param {string} package_id A string that uniquely identifies the installed package on the disk. Pass in the **packageIdentifier** field from the ${struct.PackageDetails} struct returned from the ${function.ms_iap_EnumeratePackages} async callback.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The string value `"ms_iap_MountPackage_result"`.
 * @member {real} id The asynchronous request ID.
 * @member {string} package_id A string that uniquely identifies the installed package on disk.
 * @member {string} mount_path The path to the mounted installation.
 * @event_end
 * 
 * @example
 * ```gml
 * var _package = global.package[0].packageId;
 * 
 * requestId = ms_iap_MountPackage(_package);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and after that we execute the function passing in an array of packages to be downloaded and installed. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_MountPackage_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         show_debug_message("Package ID: " + async_load[? "package_id"]);
 *         show_debug_message("Mount Path: " + async_load[? "mount_path"]);
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the current package's ID and its mount path on disk.
 * @function_end
 */

/**
 * @function ms_iap_QueryAddOnLicenses
 * @desc Retrieves the licenses the user was granted for Add-ons (also known as a durable without bits) of the currently running game. It is generally recommended to use DLC, rather than add-ons, but this API exists for the few that choose to use add-ons anyways.
 * Add-ons are typically content or features that require a purchase to unlock but don't require a download because they are built into the game. They do not work for games that have discs, or may ever have discs.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @member {string} type The constant `"ms_iap_QueryAddOnLicenses_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.AddonLicenseDetails]} result An array of ${struct.AddonLicenseDetails} structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * 
 * requestId = ms_iap_QueryAddOnLicenses(_userId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we call the function with it. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryAddOnLicenses_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console if the result of the query; an array of ${struct.AddonLicenseDetails} structs.
 * @function_end
 */


/**
 * @function ms_iap_QueryAssociatedProducts
 * @desc Gets store listing information for the products that can be purchased from within the current game. This API will only return products that can be browsed within the store including any Add-on products that do not have a store page as long as they are not expired. Any product that is hidden, expired, or taken down from the store will not be returned in the results from this API. If you need store info from a hidden, expired, or taken down product use the ${function.ms_iap_QueryProducts} passing in the product's StoreID.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {constant.ProductKind} product_kinds The types of product to return. For more information read the ${constant.ProductKind} section.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_AcquireLicenseForDurables_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.ProductDetails]} results An array of ProductDetails structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _product_kinds = e_ms_iap_ProductKind_Consumable;
 * 
 * requestId = ms_iap_QueryAssociatedProducts(_userId, _product_kinds);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user, afterwards we create a filter for consumable products (`_product_kinds`) and we call the function with both arguments. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryAssociatedProducts_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id** , printing to the debug console the result of the query; an array of [Product Details](#Product-Details) structs.
 * @function_end
 */

/**
 * @function ms_iap_QueryConsumableBalanceRemaining
 * @desc This function gets the consumable balance remaining for the specified product ID.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The ID of the consumable to retrieve the balance for.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryConsumableBalanceRemaining_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {string} store_id The ID of the consumable to retrieve the balance for.
 * @member {real} quantity The remaining quantity of the consumable.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[3].storeId;
 * 
 * requestId = ms_iap_QueryConsumableBalanceRemaining(_userId, _storeId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we get the store ID (`_storeId`) of the product we want to query the balance for (note that for convenience we keep the products stored in a global variable). The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryConsumableBalanceRemaining_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             var _quantity = async_load[? "quantity"];
 *             show_debug_message("Amount of potions left: " + string(_quantity));
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, providing a success message with the current available quantity given that the **status** value is true.
 * @function_end
 */

/**
 * @function ms_iap_QueryEntitledProducts
 * @desc This function provides the store product information for all add-ons, DLC, and consumables related to the current game that the user has an entitlement to.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {constant.ProductKind} product_kinds The types of product to return.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryEntitledProducts_result"`
 * @member {pointer} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.ProductDetails]} results An array of ProductDetails structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _product_kinds = e_ms_iap_ProductKind_Durable;
 * 
 * requestId = ms_iap_QueryEntitledProducts(_userId, _product_kinds);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user, afterwards we create a filter for durable products (`_product_kinds`) and we call the function with both arguments. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryEntitledProducts_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the result of the query; an array of ${struct.ProductDetails} structs.
 * @function_end
 */

/**
 * @function ms_iap_QueryGameLicense
 * @desc This function retrieves information about the license that was acquired to allow the app to launch.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryGameLicense_result"`
 * @member {pointer} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {real} expirationDate Expiration date of the license.
 * @member {bool} isActive Indicates whether the license is active.
 * @member {bool} isTrial Indicates whether the license is a trial license.
 * @member {bool} isTrialOwnedByTheUser Indicates whether the trial is owned by the associated user. If on PC, this will be the currently signed in user to the Windows Store App.
 * @member {bool} isDiscLicense Indicates whether the license is a disc license.
 * @member {string} skuStoreId The store ID.
 * @member {string} trialUniqueId The unique ID for the trial.
 * @member {real} trialTimeRemainingInSeconds Amount of time remaining for the trial license.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * 
 * requestId = ms_iap_QueryGameLicense(_userId);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user and we call the function with it. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryGameLicense_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "skuStoreId"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the **skuStoreId** of the current game license.
 * @function_end
 */

/**
 * @function ms_iap_QueryProductForCurrentGame
 * @desc This function provides store product information for the currently running game, such as its SKUs, availabilities and other metadata.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryProductForCurrentGame_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.ProductDetails]} results An array of ProductDetails structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * 
 * requestId = ms_iap_QueryProductForCurrentGame(_userId);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user and we call the function with it. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryProductForCurrentGame_result")
 * {
 *     if (async_load[? "async_id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the result of the query; an array of ${struct.ProductDetails} structs.
 * @function_end
 */

/**
 * @function ms_iap_QueryProductForPackage
 * @desc This function retrieves store product information for the specified package.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id A string that uniquely identifies a store package.
 * @param {constant.ProductKind} product_kinds The types of product to find.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryProductForPackage_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.ProductDetails]} results An array of ProductDetails structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _packageId = "XXXXXXXXXXX";
 * var _product_kinds = e_ms_iap_ProductKind_Consumable;
 * 
 * requestId = ms_iap_QueryProductForPackage(_userId, _packageId, _product_kinds);
 * ```
 * In the code above first we get the user ID (`_userId`) of the activating user, we reference the package ID (`_packageId`) of the target game and afterwards we create a filter for consumable products (`_product_kinds`) finally we call the function with all those arguments. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryProductForPackage_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the result of the query; an array of ${struct.ProductDetails} structs.
 * @function_end
 */

/**
 * @function ms_iap_QueryProducts
 * @desc Returns listing information for the specified products that are associated with the current game, regardless of whether the products are currently available for purchase within the current game.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {constant.ProductKind} product_kinds The types of product to return.
 * @param {array[string]} store_ids Restricts the results to the given product IDs.
 * @param {array[string]} action_filters Restricts the results by some action stored in the product document. By default, this API returns all products, even if they are not purchasable, but you can restrict this to *"Purchase"* if you only want purchasable, or *"License"* if you only want licensable. Other action filters include *"Fulfill"* , *"Browse"* , *"Curate"* , *"Details"* , and *"Redeem"*.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_QueryProducts_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {array[struct.ProductDetails]} results An array of ProductDetails structs.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _product_kinds = e_ms_iap_ProductKind_Consumable;
 * var _action_filter = ["Purchase"];
 * 
 * requestId = ms_iap_QueryProducts(_userId, _product_kinds, 0, _action_filter);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user, afterwards we create a filter for consumable products (`_product_kinds`) and finally we call the function with all those arguments, providing no store ID filter but selecting only products that are available for purchase. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_QueryProducts_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message(async_load[? "result"]);
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the result of the query; an array of ${struct.ProductDetails} structs.
 * @function_end
 */

/**
 * @function ms_iap_ReleaseLicenseForDurables
 * @desc This function will release a package license assigned to this console and it is intended to be be used with products of type **Durable**. For  **Durable with package** type, use ${function.ms_iap_ReleaseLicenseForPackage} instead.
 * 
 * @param {string} store_id The store ID of the product.
 * 
 * @returns {real} (-1 if there was an error, otherwise 0)
 * 
 * @example
 * ```gml
 * var _storeId = global.products[0].storeId;
 * 
 * var _error = ms_iap_ReleaseLicenseForDurables(_storeId);
 * 
 * if (_error != 0)
 * {
 *     show_debug_message("There was an error trying to release the license");
 * }
 * ```
 * In the code above first we get the product ID (`_storeId`) conveniently stored inside a global array and call the function with this value. The returned value (`_error`) is then checked to see if the function was successful or not.
 * @function_end
 */

/**
 * @function ms_iap_ReleaseLicenseForPackage
 * @desc This function will release a package license assigned to this console and it is intended to be be used with products of type **Durable with package** . For  **Durable**  type, use ${function.ms_iap_ReleaseLicenseForDurables} instead.
 * 
 * @param {string} package_id The store ID of the product.
 * 
 * @returns {real} (-1 if there was an error, otherwise 0)
 * 
 * @example
 * ```gml
 * var _packageId = global.packages[0].packageId;
 * 
 * var _error = ms_iap_ReleaseLicenseForPackage(_packageId);
 * 
 * if (_error != 0)
 * {
 *     show_debug_message("There was an error trying to release the license for the package!");
 * }
 * ```
 * In the code above we first get the package ID (`_packageId`) conveniently stored inside a global array and call the function with this value. The returned value (`_error`) is then checked to see if the function was successful or not.
 * @function_end
 */

/**
 * @function ms_iap_ReportConsumableFulfillment
 * @desc Consumes the specified quantity of a consumable. See [Consumable-based ecosystems](https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/commerce/fundamentals/xstore-consumable-based-ecosystems) for more information on implementing and using consumable products.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The Store ID of the consumable add-on that you want to report as fulfilled.
 * @param {real} quantity The number of units of the consumable add-on that you want to report as fulfilled. For a Store-managed consumable (that is, a consumable where Microsoft keeps track of the balance), specify the number of units that have been consumed. For a game-managed consumable (that is, a consumable where the developer keeps track of the balance), specify 1.
 * @param {string} tracking_id A developer-supplied GUID that identifies the specific transaction that the fulfillment operation is associated with for tracking purposes.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The constant `"ms_iap_ReportConsumableFulfillment_result"`
 * @member {real} id The asynchronous request ID.
 * @member {bool} status Whether or not the asynchronous request succeeded.
 * @member {string} store_id The store ID of the product.
 * @member {real} consumed_quantity The amount of product that was consumed.
 * @member {real} available_quantity The amount of product that still remains in the user's possession.
 * @event_end
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * var _trackingId = guid_generate(); // This function generates a unique identifier for tracking purposes.
 * 
 * requestId = ms_iap_ReportConsumableFulfillment(_userId, _storeId, 10, _trackingId);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user and we get the store ID (`_storeId`) of the product we want to report as consumed (note that for convenience we keep the products stored in a global variable) and finally we generate a tracking ID (`_trackingId`) required to perform the request. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_ReportConsumableFulfillment_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         if (async_load[? "status"])
 *         {
 *             show_debug_message("We consumed: " + string(async_load[? "consumed_quantity"]));
 *             show_debug_message("We still have: " + string(async_load[? "available_quantity"]));
 *         }
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **async_id** , printing to the debug console the current consumed and available quantities upon a successful task.
 * @function_end
 */

/**
 * @function ms_iap_ShowAssociatedProductsUI
 * @desc This function will open up the Microsoft Store App and show the set of available add-ons associated with the game. This can be further filtered by product type.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The store ID of the product.
 * @param {constant.ProductKind} product_kinds The types of product to show.
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.gameStoreId;
 * var _product_kinds = e_ms_iap_ProductKind_Consumable;
 * 
 * ms_iap_ShowAssociatedProductsUI(_userId, _storeId, _product_kinds);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user, we reference the store ID (`_storeId`) of current game (stored inside a global variable) and afterwards we create a filter for consumable products (`_product_kinds`) finally we call the function with all those arguments. The function call will bring up the Microsoft Store App overlay with the associated consumable products.
 * @function_end
 */

/**
 * @function ms_iap_ShowProductPageUI
 * @desc This function will open up the Store app directly to the **Product Details Page** (PDP) of the provided product. This allows titles that have not integrated with the purchase flow or an in-game store UI to still drive users to products related to their title and the purchase flow found on the Product Details Page.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The store ID of the product.
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * 
 * ms_iap_ShowProductPageUI(_userId, _storeId);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user, we reference the store ID (`_storeId`) of product we want to check and finally we call the function with both arguments. The function call will bring up the Microsoft Store App overlay directly on the **Product Details Page** (PDP) of the provided Product ID.
 * @function_end
 */

/**
 * @function ms_iap_ShowPurchaseUI
 * @desc This function begins the purchase UI overlay for the specified product.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} store_id The store ID of the product.
 * @param {string} name Name of the product to purchase.
 * @param {string} json A JSON blob (JSON-formatted string) that is handed to the purchase flow. Allows for insertion of custom campaign IDs, so you can track how the purchase started.
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _storeId = global.products[0].storeId;
 * 
 * ms_iap_ShowPurchaseUI(_userId, _storeId, undefined, undefined);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user, we reference the store ID (`_storeId`) of the product we want to purchase and finally we call the function with both arguments (providing no name/json, they are not demanding). The function call will bring up the purchase UI overlay for the specified product.
 * @function_end
 */

/**
 * @function ms_iap_ShowRateAndReviewUI
 * @desc Displays a system dialog to pop up to allow the user to provide a review for the current game or decline to do so.
 * 
 * [[Note: If the system detects a game is calling this excessively, it will hide the dialog.]]
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * 
 * ms_iap_ShowRateAndReviewUI(_userId);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user and finally we call the function with it's value. The function call will show a system dialog pop up to allow the user to provide a review for the current game.
 * @function_end
 */

/**
 * @function ms_iap_ShowRedeemTokenUI
 * @desc This function triggers a token redemption for a given user and specified token.
 * 
 * @param {pointer} user_id The user ID to use in the store context.
 * @param {string} token The token to redeem. This value cannot be undefined, if you want to bring up the UI without providing a code to pre-populate in the UI pass in a **single space**.
 * @param {array[string]} allowed_store_ids An array of product store IDs. This allows you to restrict the 5x5 codes to only work with specific products.
 * @param {bool} disallow_cvs_redemption Prevents CVS (giftcard/money style 5x5s) from being redeemed.
 * 
 * @example
 * ```gml
 * var _userId = xboxone_get_activating_user();
 * var _token = global.finalWeaponToken;
 * 
 * ms_iap_ShowRateAndReviewUI(_userId, _token, undefined, false);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user and reference a token to be redeemed (`_token`), finally we call the function with those values, applying no restrictions whatsoever. The function call will trigger a token redemption for a given user and specified token.
 * @function_end
 */

/**
 * @function ms_iap_UnmountPackage
 * @desc This function unmounts the installation of specified content.
 * 
 * @param {string} package_id A string that uniquely identifies the installed package on the disk. Pass in the **packageIdentifier** field from the ${struct.PackageDetails} struct returned from the ${function.ms_iap_EnumeratePackages} async callback.
 * 
 * @returns {real} (In-App Purchase Request ID)
 * 
 * @event iap
 * @desc 
 * @member {string} type The string value `"ms_iap_UnmountPackage_result"`.
 * @member {real} id The asynchronous request ID.
 * @member {string} package_id A string that uniquely identifies the installed package on the disk.
 * @member {string} mount_path The path to the unmounted installation.
 * @event_end
 * 
 * @example
 * ```gml
 * var _package = global.package[0].packageId;
 * 
 * requestId = ms_iap_UnmountPackage(_package);
 * ```
 * In the code above we first get the user ID (`_userId`) of the activating user and after that we execute the function passing in an array of packages to be downloaded and installed. The function call will then return a request ID (`requestId`) that can be used inside an ${event.iap}.
 * ```gml
 * if (async_load[? "type"] == "ms_iap_UnmountPackage_result")
 * {
 *     if (async_load[? "id"] == requestId)
 *     {
 *         show_debug_message("Package ID: " + async_load[? "package_id"]);
 *         show_debug_message("Finished unmount!");
 *     }
 * }
 * ```
 * The code above matches the response against the correct event **type** and **id**, printing to the debug console the current package ID and a success message.
 * @function_end
 */

/**
 * @struct AddonLicenseDetails
 * @desc This struct contains details about an add-on license.
 * 
 * The entity described above is a **struct** meaning its properties can be accessed using the **dot** operator much like when accessing instance variables, for example:
 * ```gml
 * var _addonLicenseDetails = global.addonLicenses[0];
 * 
 * show_debug_message(_packageDetails.packageIdentifier);
 * show_debug_message(_packageDetails.version);
 * ```
 * The code above will grab a package detail struct from a global variable (where they were previously store for this sample) and we are accessing the **packageIdentifier** and **version** properties of that struct.
 * 
 * @member {string} skuStoreId The SKU ID for the license.
 * @member {bool} isActive Indicates if the license is active.
 * @member {real} expirationDate Expiration date of the license ( **-1** , if the license doesn't expire)
 * @member {string} inAppOfferToken The title-defined offer token that you can use to map items internally. For example: *"com.company.product.itemname"*.
 * 
 * @struct_end
 */

/**
 * @struct PackageDetails
 * @desc This struct contains details about an installation.
 * 
 * The entity described above is a **struct** meaning its properties can be accessed using the **dot** operator much like when accessing instance variables, for example:
 * ```gml
 * var _packageDetails = global.packageDetails[0];
 * 
 * show_debug_message(_packageDetails.packageIdentifier);
 * show_debug_message(_packageDetails.version);
 * ```
 * The code above will grab a package detail struct from a global variable (where they were previously store for this sample) and we are accessing the **packageIdentifier** and **version** properties of that struct.
 * 
 * @member {string} packageIdentifier A string that uniquely identifies the installed package on the disk.
 * @member {string} version A store managed consumable product.
 * @member {constant.PackageKind} kind The value that indicates whether the package is an app package or a content package
 * @member {string} displayName The display name.
 * @member {string} description The description of the package.
 * @member {string} publisher The publisher of the package.
 * @member {string} storeId The unique ID of the product.
 * @member {bool} installing The bool that indicates whether the package is currently installing.
 * 
 * @struct_end
 */

/**
 * @struct ProductDetails
 * @desc This struct contains details that describe a store product.
 * 
 * @member {string} storeId The product ID.
 * @member {string} title The title of the product.
 * @member {string} description A description of the store product.
 * @member {string} language The International Organization of Standards (ISO) identifier representing the language the title and description strings are<br> ([more details](https://msdn.microsoft.com/library/ms693062(v=vs.85).aspx))
 * @member {string} inAppOfferToken Game defined offer token that you can use to map items internally. For example: " *com.company.product.itemname* ".
 * @member {string} linkUri The URI to the product.
 * @member {constant.ProductKind} productKind Indicates the type of store product. For more information read the [Product Kinds](#Product-Kinds) section.
 * @member {struct.Price} price The price information for the store product (read **Price** below)
 * @member {bool} hasDigitalDownload Indicates whether the store product has a digital download.
 * @member {bool} isInUserCollection Indicates if the product is in the user collection.
 * @member {string} keywords ]|Keywords associated with the store product.
 * @member {array} images Array of images associated with the product (read **Image** below)
 * @struct_end
 */
/**
 * @struct Price
 * @desc Price details inside the product struct use their own **struct** with data, following the schema below:
 * 
 * @member {real} basePrice The normal non-promotional price or MSRP of the product.
 * @member {real} price The actual price that the user would pay if they purchased the item.
 * @member {real} recurrecePrice The recurrence price.
 * @member {string} currencyCode The currency code for the price.
 * @member {string} formattedBasePrice The formatted basePrice that can be shown in the game's UI.
 * @member {string} formattedPrice The formatted price that should be used in your UI to advertise the product.
 * @member {string} formattedRecurrentPrice The formatted recurrence price.
 * @member {bool} isOnSale Indicates whether the product is on sale.
 * @member {real} saleEndData The end date for the sale.
 * @struct_end

/**
 * @struct Image
 * @desc Image details inside the product struct use their own array of **structs** with data, following the schema below:
 * 
 * The entity described above is a **struct** meaning its properties can be accessed using the **dot** operator much like when accessing instance variables, for example:
 * ```gml
 * var _productDetails = global.productDetails[0];
 * 
 * show_debug_message(_productDetails.description);
 * show_debug_message(_productDetails.price.basePrice);
 * ```
 * The code above will grab a product detail struct from a global variable (where they were previously store for this sample) and we are accessing the **description** text and **basePrice** (inside the price struct) **** properties of that struct.
 * 
 * @member {real} uri The URI to the image.
 * @member {real} height The height of the image.
 * @member {real} width The width of the image.
 * @member {string} caption The caption for the image.
 * @member {string} imagePurposeTag A string containing a tag indicating the purpose of the image.
 * 
 * @struct_end
 */

/**
 * @constant PackageKind
 * @desc Package kind indicates the package type. They are used as a filter for the ${function.ms_iap_EnumeratePackages} function to acquire information about packages of a certain type. It is also a member of the ${struct.PackageDetails} struct which describes a store product.
 * @member e_ms_iap_PackageKind_Game The installation package contains a game.
 * @member e_ms_iap_PackageKind_Content The installation package contains downloadable content.
 * @constant_end
 */

/**
 * @constant PackageScope
 * @desc Package scope indicates the scope of packages to be returned when installation packages are being enumerated while using the ${function.ms_iap_EnumeratePackages} function call.
 * 
 * @member e_ms_iap_PackageEnumerationScope_ThisOnly Scope is limited to just apps or content associated with the calling process.
 * @member e_ms_iap_PackageEnumerationScope_ThisAndRelated Scope includes apps or content associated with the calling process and also includes apps or content that are associated with any package the calling process has added to its RelatedProducts section of its game config file.
 * @constant_end
 */

/**
 * @constant ProductKind
 * @desc Product kinds indicate the product type. They are used as a filter for many IAP queries to acquire information about products of a certain type. It is also a member of the ${struct.ProductDetails} struct which describes a store product. The product kind is represented as a flagged constant meaning it can be combined using the **bit-wise or operator** to represent multiple types of product at once.
 * 
 * As explained above the product kinds can be combined using the **bit-wise** or operator following the example below:
 * ```gml
 * var _consumableAndDurableType = e_ms_iap_ProductKind_Consumable | e_ms_iap_ProductKind_Durable;
 * ```
 * The code above will make it so the `_consumableAndDurableType` variable will filter both consumables and durables.
 * 
 * @member e_ms_iap_ProductKind_None Not a product type.
 * @member e_ms_iap_ProductKind_Consumable A store-managed consumable product.
 * @member e_ms_iap_ProductKind_Durable Durable product.
 * @member e_ms_iap_ProductKind_Game A game.
 * @member e_ms_iap_ProductKind_Pass A pass.
 * @member e_ms_iap_ProductKind_UnmanagedConsumable A game-managed consumable product, also known as an unmanaged consumable.
 * 
 * @constant_end
 */

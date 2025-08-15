<a id="top"></a>
<a id="top"></a>
<!-- Page HTML do not touch -->
<a /><p align="right">[Back To Top](#top)</p>

# GDK Extension API

  This is a collection of GameMaker functions for interacting with the GDK Extension API. This API is comprised of modules that will allow you to access multiple Xbox Live features such as profile information, stats, leaderboards, achievements, storage and Microsoft Store add-ons (also known as In-App Purchases).

## Management

  Using the GDK Extension **requires** the developer to manually manage it. The following functions are provided for managing the extension:

* [gdk_init](#gdk_init)
* [gdk_update](#gdk_update)
* [gdk_quit](#gdk_quit)

## Modules

  Please look at the following sections for information on the different modules present in this library:

* [Base Module](Base-Module#Base_Module) (User)
* [Storage Module](Storage-Module#Storage_Module) (Save &amp; Load)
* [Xbox Live Module](Xbox-Live-Module#Xbox_Live_Module) (Stats &amp; Achievements)
* [IAP Module](IAP-Module#IAP_Module) (In-App Purchases)


<br><br>

---

<!-- Page HTML do not touch -->
<a /><p align="right">[Back To Top](#top)</p>

# gdk_init

  This function must be called before any other GDK extension function. It is recommended to use a persistent controller/manager object that is created or placed in the first room and to then call this function in its [Create Event](https://manual-en.yoyogames.com/The_Asset_Editors/Object_Properties/Object_Events.htm).

<br>

**Syntax:**

```gml
gdk_init(scid);
```

|Argument|Type|Description|
|----|----|----|
|scid|string|The service configuration ID (found under **Game setup** on the Partner Center).|

<br>

**Returns:**

```gml
N/A
```

<br>

**Example:**

```gml
gdk_init("00000000-0000-0000-0000-000060ddb039");
```
  The code above initialises the GDK Extension with the provided **SCID** string. This value can be acquired from your `MicrosoftGame.Config` file (that needs to be placed in the included files) or from the Partner Center.


<br><br>

---

<!-- Page HTML do not touch -->
<a /><p align="right">[Back To Top](#top)</p>

# gdk_update

  This should be called each frame while GDK extension is active, recommend using a Controller persistent object that is created or placed in the first room and this call is in the [Step Event](https://manual-en.yoyogames.com/The_Asset_Editors/Object_Properties/Object_Events.htm#).

<br>

**Syntax:**

```gml
gdk_update();
```

<br>

**Returns:**

```gml
N/A
```

<br>

**Example:**

```gml
gdk_update();
```
  In the code above we are ticking the update logic of the GDK extension, faling to call this function will make the extension stop working.


<br><br>

---

<!-- Page HTML do not touch -->
<a /><p align="right">[Back To Top](#top)</p>

# gdk_quit

  This should be called on close down and no GDK extension functions should be called after it, recommend using a Controller (persistent) object that is created or placed in the first room and this call is in its [Destroy Event](https://manual-en.yoyogames.com/The_Asset_Editors/Object_Properties/Object_Events.htm).

<br>

**Syntax:**

```gml
gdk_quit();
```

<br>

**Returns:**

```gml
N/A
```

<br>

**Example:**

```gml
gdk_quit();
```
  In the code above will terminate the GDK extension and no GDK extension functions should be called after it.


<br><br>

---
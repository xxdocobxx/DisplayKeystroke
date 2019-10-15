# DisplayKeystroke
Display keyboard and mouse input onto browsers and OBS browser source.
 
*Always remember that do not type passwords when broadcasting.*

## Screenshots
![Screenshots](https://github.com/xxdocobxx/DisplayKeystroke/raw/master/assets/screenshot001.jpg)

## Instructions
 1. Run the application `KeystrokeServer.exe` and click the `Start` button.
 2. Open the `index.html` on the browser or the OBS browser source.
 3. If you have not set the client ip on the application, you will then receive a connection request from the browser to the application. Otherwise,  the application will auto approve any request from the client ip you have provided.

## Choose a layout from preset
 1. Almost the same as above except step 2, open one of the `.html` file from the `layout` folder on the browser or the OBS browser source.

## Customize layout
If you are familiar with javascript, it is easy to create your own layout.
 - Open `layout` folder and make a copy of `QWERTY+mouse` folder and `QWERTY+mouse.html` then rename them to the new names you choose. To make it simply on this tutorial, we will name the folder to `example_layout` and the html file to `example_layout.html`.
 - Open `example_layout.html` with any text editor.
 - Find this line `<meta http-equiv='refresh' content='0; url=../index.html?layout=QWERTY+mouse'>`. Change `QWERTY+mouse` to `example_layout`, so the new line will become `<meta http-equiv='refresh' content='0; url=../index.html?layout=example_layout'>`
 - Open `layout.js` from `example_layout` folder with any text editor.
 - Remove unwanted key objects from the `items` array. A key object includes an `image` and a `text` object, it should look like `{ image: {...}, text: { key_code: '1b', ...} }`.
 - Set the properties of a key object to change the appearance of a key.

 Available key object properties:
```javascript
{
	image:
	{
		x, // image position x
		y, // image position y
		w, // image width
		h, // image height
		url, // image path name when key released
		pressed_url, // image path name when key pressed
	},
	text:
	{
		key_code, // the virtual key code, add '1' prefix if it is an extended key
		key_code2, // some of the numpad keys have different key codes when switching num lock
		text, // text displayed on the key
		x, // text position x relatively to the image position
		y, // text position y relatively to the image position
		size, // font size
		bold, // set it to true or false to toggle bold font
		font, // font name
		color, // text color when key released
		pressed_color, // text color when key pressed
	}
}
```
 *If a key object property is not set, it will assume to use the default one.*

## Demonstration
[![Demonstration](https://github.com/xxdocobxx/DisplayKeystroke/raw/master/assets/demonstration.jpg)](https://youtu.be/mq7oIN9tlfA "Demonstration")

## Buy me a coffee?
It is not required to do so but if you think that this app is useful to you and are likely to consider a tip as a small and unexpected bonus, here is the link: https://twitch.streamlabs.com/xxdocobxx

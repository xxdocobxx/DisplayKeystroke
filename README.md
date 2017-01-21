# DisplayKeystroke
Display keyboard and mouse input onto browsers and OBS browser source.

##Screenshots
![Screenshots](https://github.com/xxdocobxx/DisplayKeystroke/raw/master/assets/screenshot001.jpg)

## Instructions
 1. Run the application `DisplayKeystroke.exe`.
 2. Open the `index.html` on the browser or the OBS browser source.
 3. If you have not set the client ip on the application, you will then receive a request connection from the browser to the application. Otherwise,  the application will auto approve any request from the client ip you have provided.

##Choose a layout from preset
 1. Almost the same as above except step 2, open one of the `.html` file from the `layout` folder on the browser or the OBS browser source.

##Customize layout

If you are familiar with javascript, it is easy to create your own layout.
 - Open `layout` folder and make a copy of `QWERTY+mouse` folder and `QWERTY+mouse.html` then rename them to the new names you choose. To make it simply on this tutorial, we will name the folder to `example_layout` and the html file to `example_layout`.html`.
 - Open `example_layout.html`. Find this line `<meta http-equiv='refresh' content='0; url=../index.html?layout=QWERTY+mouse'>`. Then we change `QWERTY+mouse` to `example_layout`, so the new line will become `<meta http-equiv='refresh' content='0; url=../index.html?layout=example_layout'>`
 - Open `layout.js` from `example_layout` folder with any text editor.
 - Remove unwanted key objects from the `items` array. A key object includes a `image` and a `text` object, it should look like `{ image: {...}, text: { key_code: '1b', ...} }`.
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
		url, // image path when key released
		pressed_url, // image name when key pressed
	},
	text:
	{
		x, // text position x relatively to the image position
		y, // text position y relatively to the image position
		size, // font size
		bold, // set it to true or false to toggle bold font weight
		font, // font name
		color, // text color when key released
		pressed_color, // text color when key pressed
	}
}
```
 *If a key object property is not set, it will assume to use the default one.*

##Demonstration
 
[![Demonstration](https://github.com/xxdocobxx/DisplayKeystroke/raw/master/assets/demonstration.jpg)](https://youtu.be/s5DkyKBpg9E "Demonstration")
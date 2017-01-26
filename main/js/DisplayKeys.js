function hasProperty(obj, prop)
{
	return (obj.hasOwnProperty(prop) && obj.prop !== '');
}

function getParameter(val)
{
	var regex = new RegExp('[\\?&#]' + val + '=([^&#]*)');
	var para = regex.exec(document.location.search);

	return para ? decodeURIComponent(para[1]) : null;
}

// custom layout

var layout_folder_name = '';

function getCustomLayout()
{
	layout_folder_name = getParameter('layout');
	
	layout_folder_name = 'layout/' + (layout_folder_name === null || layout_folder_name === '' ? 'QWERTY+mouse' : decodeURIComponent(layout_folder_name));
	
	if($('#load_layout_script').length)
		$('#load_layout_script').remove();

	if(layout_folder_name.charAt(layout_folder_name.length - 1) !== '/')
		layout_folder_name += '/';

	var script = $('<script>').prop
	({
		src: layout_folder_name + 'layout.js',
		id: 'load_layout_script',
		type: 'text/javascript',
		async: false
	});

	document.head.appendChild(script[0]);
}

function loadLayout(layout)
{
	var display_area = $('#display-area');
	display_area.empty();

	var style = '';

	if(hasProperty(layout, 'default_image'))
	{
		style +=
			'.key-image' +
			'{' +
				'background-size:100% 100%;' +
				'background-repeat:no-repeat;' +
				(hasProperty(layout.default_image, 'url') ? 'background-image:url(\'' + layout_folder_name + layout.default_image.url + '\');' : '' ) +
				(hasProperty(layout.default_image, 'x') ? 'left:' + layout.default_image.x + 'px;' : '' ) +
				(hasProperty(layout.default_image, 'y') ? 'top:' + layout.default_image.y + 'px;' : '' ) +
				(hasProperty(layout.default_image, 'w') ? 'width:' + layout.default_image.w + 'px;' : '' ) +
				(hasProperty(layout.default_image, 'h') ? 'height:' + layout.default_image.h + 'px;' : '' ) +
			'}' +
			'.pressed>.key-image' +
			'{' +
				(hasProperty(layout.default_image, 'pressed_url') ? 'background-image:url(\'' + layout_folder_name  + layout.default_image.pressed_url + '\');' : '' ) +
			'}';
	}

	if(hasProperty(layout, 'default_text'))
	{
		style +=
			'.key-text' +
			'{' +
				(hasProperty(layout.default_text, 'font') ? 'font-family:' + layout.default_text.font + ';' : '' ) +
				(hasProperty(layout.default_text, 'size') ? 'font-size:' + layout.default_text.size + 'px;' : '' ) +
				(hasProperty(layout.default_text, 'bold') ? 'font-weight:' + (layout.default_text.bold ? 'bold' : 'normal') + ';' : '' ) +
				(hasProperty(layout.default_text, 'x') ? 'left:' + layout.default_text.x + 'px;' : '' ) +
				(hasProperty(layout.default_text, 'y') ? 'top:' + layout.default_text.y + 'px;' : '' ) +
				(hasProperty(layout.default_text, 'color') ? 'color:' + layout.default_text.color + ';' : '' ) +
			'}' +
			'.pressed .key-text' +
			'{' +
				(hasProperty(layout.default_text, 'pressed_color') ? 'color:' + layout.default_text.pressed_color + ';' : '' ) +
			'}';
	}

	for(var i = 0; i < layout.items.length; ++i)
	{
		var item = layout.items[i];

		var image_css = {};
		if(hasProperty(item.image, 'x'))
			image_css.left = item.image.x;
		if(hasProperty(item.image, 'y'))
			image_css.top = item.image.y;
		if(hasProperty(item.image, 'w'))
			image_css.width = item.image.w;
		if(hasProperty(item.image, 'h'))
			image_css.height = item.image.h;
		if(hasProperty(item.image, 'url'))
			style +=
				'.key-' + item.text.key_code + '>.key-image' +
				'{background-image:url(\'' + layout_folder_name  + item.image.url + '\');}';
		if(hasProperty(item.image, 'pressed_url'))
			style +=
				'.key-' + item.text.key_code + '.pressed>.key-image' +
				'{background-image:url(\'' + layout_folder_name  + item.image.pressed_url + '\');}';

		var text_css = {};
		if(hasProperty(item.text, 'x'))
			text_css.left = item.text.x;
		if(hasProperty(item.text, 'y'))
			text_css.top = item.text.y;
		if(hasProperty(item.text, 'size'))
			text_css['font-size'] = item.text.size;
		if(hasProperty(item.text, 'bold'))
			text_css['font-weight'] = item.text.bold ? 'bold' : 'normal';
		if(hasProperty(item.text, 'font'))
			text_css['font-family'] = item.text.font;
		if(hasProperty(item.text, 'color'))
			style +=
				'.key-' + item.text.key_code + ' .key-text' +
				'{color:' + item.text.color + ';}';
		if(hasProperty(item.text, 'pressed_color'))
			style +=
				'.key-' + item.text.key_code + '.pressed .key-text' +
				'{color:' + item.text.pressed_color + ';}';

		var container = $('<div>').attr({class: 'key-container key-' + item.text.key_code + (hasProperty(item.text, 'key_code2') ? ' key-' + item.text.key_code2 : '')});
		container.append($('<div>').attr({class: 'key-image'}).css(image_css)
		.append($('<div>').attr({class: 'key-text'}).css(text_css).html(hasProperty(item.text, 'text') ? item.text.text : '')));
		display_area.append(container);
	}

	$('head').append($('<style>').attr('id', 'inputdisplay-style').html(style));
}

// KeystrokeClient events

KeystrokeClient.onConnect = function()
{
	$('#disconnect-screen').addClass('connected');
};

KeystrokeClient.onDisconnect = function()
{
	$('#disconnect-screen').removeClass('connected');
	$('.pressed').removeClass('pressed');
};

var mouse_wheel_up_timer = null;
var mouse_wheel_down_timer = null;

KeystrokeClient.onKeyDown = function(key_code)
{
	var container = $('.key-' + key_code);
	if(container.length)
	{
		container.addClass('pressed');

		switch(key_code)
		{
		case '0a': // mouse wheel up
			if(mouse_wheel_up_timer !== null)
				clearTimeout(mouse_wheel_up_timer);
			mouse_wheel_up_timer = setTimeout(function() { mouse_wheel_up_timer = null; container.removeClass('pressed'); }, 100);
			break;

		case '0b': // mouse wheel down
			if(mouse_wheel_down_timer !== null)
				clearTimeout(mouse_wheel_down_timer);
			mouse_wheel_down_timer = setTimeout(function() { mouse_wheel_down_timer = null; container.removeClass('pressed'); }, 100);
			break;
		}
	}
};

KeystrokeClient.onKeyUp = function(key_code)
{
	var container = $('.key-' + key_code);
	if(container.length)
		container.removeClass('pressed');
};

window.onload = function(e)
{
	getCustomLayout();
	KeystrokeClient.start(host_ip, host_port);
};
loadLayout(
{
	default_image: { x:0, y: 0, w: 156, h: 300 },
	items:
	[
// mouse
		{	// mouse body
			image: { url: 'pic/mouse.png' },
			text: { key_code: '00' }
		},
		{	// mouse left button
			image: { pressed_url: 'pic/mouse_l_btn.png' },
			text: { key_code: '01' }
		},
		{	// mouse right button
			image: { pressed_url: 'pic/mouse_r_btn.png' },
			text: { key_code: '02' }
		},
		{	// mouse middle button
			image: { pressed_url: 'pic/mouse_m_btn.png' },
			text: { key_code: '04' }
		},
		{	// mouse x1 button
			image: { pressed_url: 'pic/mouse_x1_btn.png' },
			text: { key_code: '05' }
		},
		{	// mouse x2 button
			image: { pressed_url: 'pic/mouse_x2_btn.png' },
			text: { key_code: '06' }
		},
		{	// mouse wheel up
			image: { pressed_url: 'pic/mouse_wheel_up.png' },
			text: { key_code: '0a' }
		},
		{	// mouse wheel down
			image: { pressed_url: 'pic/mouse_wheel_down.png' },
			text: { key_code: '0b' }
		},
	]
}
);
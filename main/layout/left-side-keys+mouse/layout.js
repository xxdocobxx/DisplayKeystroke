loadLayout(
{
	default_image: { w: 50, h: 50, url: 'pic/key.png', pressed_url: 'pic/pressed.png' },
	default_text: { x: 12, y: 6, size: 14, bold: true, font: 'Arial, Helvetica, sans-serif', color: '#ffffff', pressed_color: '#000000' },
	items:
	[
// function keys
		{
			image: { x: 0, y: 0 },
			text: { key_code: '1b', text: 'Esc', size: 12, bold: false }
		},
// Alphanumeric
		{
			image: { x: 0, y: 55 },
			text: { key_code: 'c0', text: '~<br>`' }
		},
		{
			image: { x: 50, y: 55 },
			text: { key_code: '31', text: '1' }
		},
		{
			image: { x: 100, y: 55 },
			text: { key_code: '32', text: '2' }
		},
		{
			image: { x: 150, y: 55 },
			text: { key_code: '33', text: '3' }
		},
		{
			image: { x: 200, y: 55 },
			text: { key_code: '34', text: '4' }
		},
		{
			image: { x: 0, y: 105, w: 75, url: 'pic/key_tab.png', pressed_url: 'pic/pressed_tab.png' },
			text: { key_code: '09', text: 'Tab', size: 12, bold: false }
		},
		{
			image: { x: 75, y: 105 },
			text: { key_code: '51', text: 'Q' }
		},
		{
			image: { x: 125, y: 105 },
			text: { key_code: '57', text: 'W' }
		},
		{
			image: { x: 175, y: 105 },
			text: { key_code: '45', text: 'E' }
		},
		{
			image: { x: 225, y: 105 },
			text: { key_code: '52', text: 'R' }
		},
		{
			image: { x: 0, y: 155, w: 91, url: 'pic/key_caps.png', pressed_url: 'pic/pressed_caps.png' },
			text: { key_code: '14', text: 'Caps<br>Lock', size: 12, bold: false }
		},
		{
			image: { x: 91, y: 155 },
			text: { key_code: '41', text: 'A' }
		},
		{
			image: { x: 141, y: 155 },
			text: { key_code: '53', text: 'S' }
		},
		{
			image: { x: 191, y: 155 },
			text: { key_code: '44', text: 'D' }
		},
		{
			image: { x: 241, y: 155 },
			text: { key_code: '46', text: 'F' }
		},
		{	// left shift
			image: { x: 0, y: 205, w: 120, url: 'pic/key_shift_i.png', pressed_url: 'pic/pressed_shift_i.png' },
			text: { key_code: 'a0', text: 'Shift', x: 40, y: 16, size: 12, bold: false }
		},
		{
			image: { x: 120, y: 205 },
			text: { key_code: '5a', text: 'Z' }
		},
		{
			image: { x: 170, y: 205 },
			text: { key_code: '58', text: 'X' }
		},
		{
			image: { x: 220, y: 205 },
			text: { key_code: '43', text: 'C' }
		},
		{
			image: { x: 270, y: 205 },
			text: { key_code: '56', text: 'V' }
		},
		{	// left ctrl
			image: { x: 0, y: 255, w: 75, url: 'pic/key_tab.png', pressed_url: 'pic/pressed_tab.png' },
			text: { key_code: 'a2', text: 'Ctrl', size: 12, bold: false }
		},
		{	// left win
			image: { x: 75, y: 255, w: 62, url: 'pic/key_win.png', pressed_url: 'pic/pressed_win.png' },
			text: { key_code: '15b', text: '' }
		},
		{	// left alt
			image: { x: 137, y: 255, w: 62, url: 'pic/key_alt.png', pressed_url: 'pic/pressed_alt.png' },
			text: { key_code: 'a4', text: 'Alt', size: 12, bold: false }
		},
		{	// left space
			image: { x: 199, y: 255, w: 140, url: 'pic/key_space_s.png', pressed_url: 'pic/pressed_space_s.png' },
			text: { key_code: '20', text: '' }
		},
// mouse
		{	// mouse body
			image: { x: 320, y: -15, w: 156, h: 300, url: 'pic/mouse.png', pressed_url: '' },
			text: { key_code: '00', text: '' }
		},
		{	// mouse left button
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_l_btn.png' },
			text: { key_code: '01', text: '' }
		},
		{	// mouse right button
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_r_btn.png' },
			text: { key_code: '02', text: '' }
		},
		{	// mouse middle button
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_m_btn.png' },
			text: { key_code: '04', text: '' }
		},
		{	// mouse x1 button
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_x1_btn.png' },
			text: { key_code: '05', text: '' }
		},
		{	// mouse x2 button
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_x2_btn.png' },
			text: { key_code: '06', text: '' }
		},
		{	// mouse wheel up
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_wheel_up.png' },
			text: { key_code: '0a', text: '' }
		},
		{	// mouse wheel down
			image: { x: 320, y: -15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_wheel_down.png' },
			text: { key_code: '0b', text: '' }
		},
	]
}
);
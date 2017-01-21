loadLayout(
{
	default_image: { w: 50, h: 50, url: 'pic/key.png', pressed_url: 'pic/pressed.png' },
	default_text: { x: 12, y: 6, size: 14, bold: true, font: 'Arial, Helvetica, sans-serif', color: '#ffffff', pressed_color: '#000000' },
	items:
	[
// Num pad
		{	// Num lock
			image: { x: 0, y: 0 },
			text: { key_code: '190', text: 'Num<br>Lock', size: 9, bold: false }
		},
		{	// Num /
			image: { x: 50, y: 0 },
			text: { key_code: '16f', text: '/' }
		},
		{	// Num *
			image: { x: 100, y: 0 },
			text: { key_code: '6a', text: '&#10033;' }
		},
		{	// Num -
			image: { x: 150, y: 0 },
			text: { key_code: '6d', text: '-' }
		},
		{	// Num 7
			image: { x: 0, y: 50 },
			text: { key_code: '67', key_code2: '24', text: '7<br>Home', size: 9, bold: false }
		},
		{	// Num 8
			image: { x: 50, y: 50 },
			text: { key_code: '68', key_code2: '26', text: '8<br>&#8593;', size: 9, bold: false }
		},
		{	// Num 9
			image: { x: 100, y: 50 },
			text: { key_code: '69', key_code2: '21', text: '9<br>Pg Up', size: 9, bold: false }
		},
		{	// Num +
			image: { x: 150, y: 50, h: 100, url: 'pic/key_num_add.png', pressed_url: 'pic/pressed_num_add.png' },
			text: { key_code: '6b', text: '+' }
		},
		{	// Num 4
			image: { x: 0, y: 100 },
			text: { key_code: '64', key_code2: '25', text: '4<br>&#8592;', size: 9, bold: false }
		},
		{	// Num 5
			image: { x: 50, y: 100 },
			text: { key_code: '65', key_code2: '0c', text: '5', size: 9, bold: false }
		},
		{	// Num 6
			image: { x: 100, y: 100 },
			text: { key_code: '66', key_code2: '27', text: '6<br>&#8594;', size: 9, bold: false }
		},
		{	// Num 1
			image: { x: 0, y: 150 },
			text: { key_code: '61', key_code2: '23', text: '1<br>End', size: 9, bold: false }
		},
		{	// Num 2
			image: { x: 50, y: 150 },
			text: { key_code: '62', key_code2: '28', text: '2<br>&#8595;', size: 9, bold: false }
		},
		{	// Num 3
			image: { x: 100, y: 150 },
			text: { key_code: '63', key_code2: '22', text: '3<br>Pg Dn', size: 9, bold: false }
		},
		{	// Num Enter
			image: { x: 150, y: 150, h: 100, url: 'pic/key_num_add.png', pressed_url: 'pic/pressed_num_add.png' },
			text: { key_code: '10d', text: 'Enter', size: 9, bold: false }
		},
		{	// Num 0
			image: { x: 0, y: 200, w: 100, url: 'pic/key_num0.png', pressed_url: 'pic/pressed_num0.png' },
			text: { key_code: '60', key_code2: '2d', text: '0<br>Ins', size: 9, bold: false }
		},
		{	// Num .
			image: { x: 100, y: 200 },
			text: { key_code: '6e', key_code2: '2e', text: '&#183;<br>Del', size: 9, bold: false }
		},
	]
}
);
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
                {
                    image: { x: 100, y: 0 },
                    text: { key_code: '70', text: 'F1', size: 12, bold: false }
                },
                {
                    image: { x: 150, y: 0 },
                    text: { key_code: '71', text: 'F2', size: 12, bold: false }
                },
                {
                    image: { x: 200, y: 0 },
                    text: { key_code: '72', text: 'F3', size: 12, bold: false }
                },
                {
                    image: { x: 250, y: 0 },
                    text: { key_code: '73', text: 'F4', size: 12, bold: false }
                },
                {
                    image: { x: 325, y: 0 },
                    text: { key_code: '74', text: 'F5', size: 12, bold: false }
                },
                {
                    image: { x: 375, y: 0 },
                    text: { key_code: '75', text: 'F6', size: 12, bold: false }
                },
                {
                    image: { x: 425, y: 0 },
                    text: { key_code: '76', text: 'F7', size: 12, bold: false }
                },
                {
                    image: { x: 475, y: 0 },
                    text: { key_code: '77', text: 'F8', size: 12, bold: false }
                },
                {
                    image: { x: 550, y: 0 },
                    text: { key_code: '78', text: 'F9', size: 12, bold: false }
                },
                {
                    image: { x: 600, y: 0 },
                    text: { key_code: '79', text: 'F10', size: 12, bold: false }
                },
                {
                    image: { x: 650, y: 0 },
                    text: { key_code: '7a', text: 'F11', size: 12, bold: false }
                },
                {
                    image: { x: 700, y: 0 },
                    text: { key_code: '7b', text: 'F12', size: 12, bold: false }
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
                    image: { x: 250, y: 55 },
                    text: { key_code: '35', text: '5' }
                },
                {
                    image: { x: 300, y: 55 },
                    text: { key_code: '36', text: '6' }
                },
                {
                    image: { x: 350, y: 55 },
                    text: { key_code: '37', text: '7' }
                },
                {
                    image: { x: 400, y: 55 },
                    text: { key_code: '38', text: '8' }
                },
                {
                    image: { x: 450, y: 55 },
                    text: { key_code: '39', text: '9' }
                },
                {
                    image: { x: 500, y: 55 },
                    text: { key_code: '30', text: '0' }
                },
                {
                    image: { x: 550, y: 55 },
                    text: { key_code: 'db', text: '[<br>{' }
                },
                {
                    image: { x: 600, y: 55 },
                    text: { key_code: 'dd', text: ']<br>}' }
                },
                {
                    image: { x: 650, y: 55 },
                    text: { key_code: 'dc', text: '|<br>\\' }
                },
                {
                    image: { x: 700, y: 55 },
                    text: { key_code: '08', text: '&#11013;' }
                },
                {
                    image: { x: 0, y: 105, w: 75, url: 'pic/key_tab.png', pressed_url: 'pic/pressed_tab.png' },
                    text: { key_code: '09', text: 'Tab', size: 12, bold: false }
              },
              {
                 image: { x: 75, y: 105 },
                 text: { key_code: 'de', text: '"<br>\'' }
              },
              {
                 image: { x: 125, y: 105 },
                 text: { key_code: 'bc', text: '<<br>.' }
              },
              {
                 image: { x: 175, y: 105 },
                 text: { key_code: 'be', text: '><br>.' }
              },
              {
                 image: { x: 225, y: 105 },
                 text: { key_code: '50', text: 'P' }
              },
              {
                 image: { x: 275, y: 105 },
                 text: { key_code: '59', text: 'Y' }
              },
              {
                 image: { x: 325, y: 105 },
                 text: { key_code: '46', text: 'F' }
              },
              {
                 image: { x: 375, y: 105 },
                 text: { key_code: '47', text: 'G' }
              },
              {
                 image: { x: 425, y: 105 },
                 text: { key_code: '43', text: 'C' }
              },
              {
                 image: { x: 475, y: 105 },
                 text: { key_code: '52', text: 'R' }
              },
              {
                 image: { x: 525, y: 105 },
                 text: { key_code: '4c', text: 'L' }
              },
              {
                 image: { x: 575, y: 105 },
                 text: { key_code: 'bf', text: '?<br>/' }
              },
              {
                 image: { x: 625, y: 105 },
                 text: { key_code: 'bb', text: '+<br>=' }
              },
              {
                 image: { x: 641, y: 105, w: 109, h: 100, url: 'pic/key_enter_i.png', pressed_url: 'pic/pressed_enter_i.png' },
                 text: { key_code: '0d', text: 'Enter', x: 50, y: 65, size: 12, bold: false }
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
                 text: { key_code: '4f', text: 'O' }
              },
              {
                 image: { x: 191, y: 155 },
                 text: { key_code: '45', text: 'E' }
              },
              {
                 image: { x: 241, y: 155 },
                 text: { key_code: '55', text: 'U' }
              },
              {
                 image: { x: 291, y: 155 },
                 text: { key_code: '49', text: 'I' }
              },
              {
                 image: { x: 341, y: 155 },
                 text: { key_code: '44', text: 'D' }
              },
              {
                 image: { x: 391, y: 155 },
                 text: { key_code: '48', text: 'H' }
              },
              {
                 image: { x: 441, y: 155 },
                 text: { key_code: '54', text: 'T' }
              },
              {
                 image: { x: 491, y: 155 },
                 text: { key_code: '4e', text: 'N' }
              },
              {
                 image: { x: 541, y: 155 },
                 text: { key_code: '53', text: 'S' }
              },
              {
                 image: { x: 591, y: 155 },
                 text: { key_code: 'bd', text: '-<br>\_' }
              },
              {	// left shift
                 image: { x: 0, y: 205, w: 120, url: 'pic/key_shift_i.png', pressed_url: 'pic/pressed_shift_i.png' },
                 text: { key_code: 'a0', text: 'Shift', x: 40, y: 16, size: 12, bold: false }
              },
              {
                 image: { x: 120, y: 205 },
                 text: { key_code: 'ba', text: ';<br>:'}
              },
              {
                 image: { x: 170, y: 205 },
                 text: { key_code: '51', text: 'Q' }
              },
              {
                 image: { x: 220, y: 205 },
                 text: { key_code: '4a', text: 'J' }
              },
              {
                 image: { x: 270, y: 205 },
                 text: { key_code: '4b', text: 'K' }
              },
              {
                 image: { x: 320, y: 205 },
                 text: { key_code: '58', text: 'X' }
              },
              {
                 image: { x: 370, y: 205 },
                 text: { key_code: '42', text: 'B' }
              },
              {
                 image: { x: 420, y: 205 },
                 text: { key_code: '4d', text: 'M' }
              },
              {
                 image: { x: 470, y: 205 },
                 text: { key_code: '57', text: 'W' }
              },
              {
                 image: { x: 520, y: 205 },
                 text: { key_code: '56', text: 'V' }
              },
              {
                 image: { x: 570, y: 205 },
                 text: { key_code: '5a', text: 'Z' }
              },
                {	// right shift
                    image: { x: 620, y: 205, w: 128, url: 'pic/key_shift_i.png', pressed_url: 'pic/pressed_shift_i.png' },
                    text: { key_code: '1a1', text: 'Shift', x: 42, y: 16, size: 12, bold: false }
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
                    image: { x: 199, y: 255, w: 287, url: 'pic/key_space.png', pressed_url: 'pic/pressed_space.png' },
                    text: { key_code: '20', text: '' }
                },
                {	// right alt
                    image: { x: 486, y: 255, w: 62, url: 'pic/key_alt.png', pressed_url: 'pic/pressed_alt.png' },
                    text: { key_code: '1a5', text: 'Alt', size: 12, bold: false }
                },
                {	// right win
                    image: { x: 548, y: 255, w: 62, url: 'pic/key_win.png', pressed_url: 'pic/pressed_win.png' },
                    text: { key_code: '15c', text: '' }
                },
                {	// apps
                    image: { x: 610, y: 255, w: 62, url: 'pic/key_apps.png', pressed_url: 'pic/pressed_apps.png' },
                    text: { key_code: '15d', text: '' }
                },
                {	// right ctrl
                    image: { x: 672, y: 255, w: 75, url: 'pic/key_tab.png', pressed_url: 'pic/pressed_tab.png' },
                    text: { key_code: '1a3', text: 'Ctrl', size: 12, bold: false }
                },
                // extend pad
                {
                    image: { x: 755, y: 0 },
                    text: { key_code: '12c', text: 'Print<br>Screen', size: 9, bold: false }
                },
                {
                    image: { x: 805, y: 0 },
                    text: { key_code: '91', text: 'Scroll<br>Lock', size: 9, bold: false }
                },
                {
                    image: { x: 855, y: 0 },
                    text: { key_code: '13', text: 'Pause<br>Break', size: 9, bold: false }
                },
                {
                    image: { x: 755, y: 55 },
                    text: { key_code: '12d', text: 'Insert', size: 9, bold: false }
                },
                {
                    image: { x: 805, y: 55 },
                    text: { key_code: '124', text: 'Home', size: 9, bold: false }
                },
                {
                    image: { x: 855, y: 55 },
                    text: { key_code: '121', text: 'Page<br>Up', size: 9, bold: false }
                },
                {
                    image: { x: 755, y: 105 },
                    text: { key_code: '12e', text: 'Delete', size: 9, bold: false }
                },
                {
                    image: { x: 805, y: 105 },
                    text: { key_code: '123', text: 'End', size: 9, bold: false }
                },
                {
                    image: { x: 855, y: 105 },
                    text: { key_code: '122', text: 'Page<br>Down', size: 9, bold: false }
                },
                {	// up
                    image: { x: 805, y: 205 },
                    text: { key_code: '126', text: '&#8593;' }
                },
                {	// left
                    image: { x: 755, y: 255 },
                    text: { key_code: '125', text: '&#8592;' }
                },
                {	// down
                    image: { x: 805, y: 255 },
                    text: { key_code: '128', text: '&#8595;' }
                },
                {	// right
                    image: { x: 855, y: 255 },
                    text: { key_code: '127', text: '&#8594;' }
                },
                // Num pad
                {	// Num lock
                    image: { x: 910, y: 55 },
                    text: { key_code: '190', text: 'Num<br>Lock', size: 9, bold: false }
                },
                {	// Num /
                    image: { x: 960, y: 55 },
                    text: { key_code: '16f', text: '/' }
                },
                {	// Num *
                    image: { x: 1010, y: 55 },
                    text: { key_code: '6a', text: '&#10033;' }
                },
                {	// Num -
                    image: { x: 1060, y: 55 },
                    text: { key_code: '6d', text: '-' }
                },
                {	// Num 7
                    image: { x: 910, y: 105 },
                    text: { key_code: '67', key_code2: '24', text: '7<br>Home', size: 9, bold: false }
                },
                {	// Num 8
                    image: { x: 960, y: 105 },
                    text: { key_code: '68', key_code2: '26', text: '8<br>&#8593;', size: 9, bold: false }
                },
                {	// Num 9
                    image: { x: 1010, y: 105 },
                    text: { key_code: '69', key_code2: '21', text: '9<br>Pg Up', size: 9, bold: false }
                },
                {	// Num +
                    image: { x: 1060, y: 105, h: 100, url: 'pic/key_num_add.png', pressed_url: 'pic/pressed_num_add.png' },
                    text: { key_code: '6b', text: '+' }
                },
                {	// Num 4
                    image: { x: 910, y: 155 },
                    text: { key_code: '64', key_code2: '25', text: '4<br>&#8592;', size: 9, bold: false }
                },
                {	// Num 5
                    image: { x: 960, y: 155 },
                    text: { key_code: '65', key_code2: '0c', text: '5', size: 9, bold: false }
                },
                {	// Num 6
                    image: { x: 1010, y: 155 },
                    text: { key_code: '66', key_code2: '27', text: '6<br>&#8594;', size: 9, bold: false }
                },
                {	// Num 1
                    image: { x: 910, y: 205 },
                    text: { key_code: '61', key_code2: '23', text: '1<br>End', size: 9, bold: false }
                },
                {	// Num 2
                    image: { x: 960, y: 205 },
                    text: { key_code: '62', key_code2: '28', text: '2<br>&#8595;', size: 9, bold: false }
                },
                {	// Num 3
                    image: { x: 1010, y: 205 },
                    text: { key_code: '63', key_code2: '22', text: '3<br>Pg Dn', size: 9, bold: false }
                },
                {	// Num Enter
                    image: { x: 1060, y: 205, h: 100, url: 'pic/key_num_add.png', pressed_url: 'pic/pressed_num_add.png' },
                    text: { key_code: '10d', text: 'Enter', size: 9, bold: false }
                },
                {	// Num 0
                    image: { x: 910, y: 255, w: 100, url: 'pic/key_num0.png', pressed_url: 'pic/pressed_num0.png' },
                    text: { key_code: '60', key_code2: '2d', text: '0<br>Ins', size: 9, bold: false }
                },
                {	// Num .
                    image: { x: 1010, y: 255 },
                    text: { key_code: '6e', key_code2: '2e', text: '&#183;<br>Del', size: 9, bold: false }
                },
                // mouse
                {	// mouse body
                    image: { x: 1120, y: 15, w: 156, h: 300, url: 'pic/mouse.png', pressed_url: '' },
                    text: { key_code: '00', text: '' }
                },
                {	// mouse left button
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_l_btn.png' },
                    text: { key_code: '01', text: '' }
                },
                {	// mouse right button
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_r_btn.png' },
                    text: { key_code: '02', text: '' }
                },
                {	// mouse middle button
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_m_btn.png' },
                    text: { key_code: '04', text: '' }
                },
                {	// mouse x1 button
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_x1_btn.png' },
                    text: { key_code: '05', text: '' }
                },
                {	// mouse x2 button
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_x2_btn.png' },
                    text: { key_code: '06', text: '' }
                },
                {	// mouse wheel up
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_wheel_up.png' },
                    text: { key_code: '0a', text: '' }
                },
                {	// mouse wheel down
                    image: { x: 1120, y: 15, w: 156, h: 300, url: '', pressed_url: 'pic/mouse_wheel_down.png' },
                    text: { key_code: '0b', text: '' }
                },
            ]
    }
);

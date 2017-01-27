var KeystrokeClient = new function()
{
	var _this = this;

	_this._ws = null;
	_this._reconnect_timer = null;
	_this._host_ip = null;
	_this._host_port = null;
	
	_this._resetModifierKeysStates = function()
	{
		_this._modifier_keys_states = { alt: 0, ctrl: 0, shift: 0 };
	};
	_this._resetModifierKeysStates();
	
	_this.testModifierKeys = function(alt, ctrl, shift)
	{
		return (
			((!alt && !_this._modifier_keys_states.alt) || (alt && _this._modifier_keys_states.alt)) && 
			((!ctrl && !_this._modifier_keys_states.ctrl) || (ctrl && _this._modifier_keys_states.ctrl)) && 
			((!shift && !_this._modifier_keys_states.shift) || (shift && _this._modifier_keys_states.shift))
		);
	};

	_this.start = function(host_ip, host_port)
	{
		if(typeof host_ip !== 'string' && typeof host_port !== 'string')
			return;

		if (_this._ws !== null)
		{
			_this._ws.close();
			_this._ws = null;
		}

		_this._host_ip = host_ip;
		_this._host_port = host_port;

		_this._ws = new WebSocket('ws://' + host_ip + ':' + host_port);
		_this._ws.onopen = _this._onOpen;
		_this._ws.onclose = _this._onClose;
		_this._ws.onmessage = _this._onMessage;
	};

	_this.stop = function()
	{
		_this._ws.close();
	};

	_this._onOpen = function()
	{
		_this._resetModifierKeysStates();
	};

	_this._onClose = function()
	{
		_this._ws = null;

		if(_this._reconnect_timer !== null)
			clearTimeout(_this._reconnect_timer);
		_this._reconnect_timer = setTimeout(function() { _this._reconnect_timer = null; _this.start(_this._host_ip, _this._host_port); }, 10000);

		_this._resetModifierKeysStates();

		if(typeof _this.onDisconnect === 'function')
			_this.onDisconnect();
	};

	_this._onMessage = function(e)
	{
		if(typeof e === 'object' && typeof e.data === 'string')
		{
			if(e.data.length === 4)
			{
				var toggle = (e.data[0] === '+');
				var key_code = (e.data[1] !== '0' ? '1' : '') + e.data[2] + e.data[3];

				if(toggle)
				{
					switch(key_code)
					{
					case 'a4': // left alt
						_this._modifier_keys_states.alt |= 1;
						break;
					
					case '1a5': // right alt
						_this._modifier_keys_states.alt |= 2;
						break;

					case 'a2': // left ctrl
						_this._modifier_keys_states.ctrl |= 1;
						break;
					
					case '1a3': // right ctrl
						_this._modifier_keys_states.ctrl |= 2;
						break;

					case 'a0': // left shift
						_this._modifier_keys_states.shift |= 1;
						break;
					
					case '1a1': // right shift
						_this._modifier_keys_states.shift |= 2;
						break;
					}

					if(typeof _this.onKeyDown === 'function')
						_this.onKeyDown(key_code, _this._modifier_keys_states);
				}
				else
				{
					switch(key_code)
					{
					case 'a4': // left alt
						_this._modifier_keys_states.alt &= ~1;
						break;
					
					case '1a5': // right alt
						_this._modifier_keys_states.alt &= ~2;
						break;

					case 'a2': // left ctrl
						_this._modifier_keys_states.ctrl &= ~1;
						break;
					
					case '1a3': // right ctrl
						_this._modifier_keys_states.ctrl &= ~2;
						break;

					case 'a0': // left shift
						_this._modifier_keys_states.shift &= ~1;
						break;
					
					case '1a1': // right shift
						_this._modifier_keys_states.shift &= ~2;
						break;
					}

					if(typeof _this.onKeyUp === 'function')
						_this.onKeyUp(key_code, _this._modifier_keys_states);
				}
			}
			else if(e.data === 'server approved')
			{
				if(typeof _this.onConnect === 'function')
					_this.onConnect();
			}
		}
	};
};
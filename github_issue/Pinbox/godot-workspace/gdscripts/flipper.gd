extends RigidBody2D


var _angle_offset
var _do_turn = false
var _do_turn_check = false

@export
var _prevention = true

@export
var _use_right_hand_input = false

@export
var _is_right_hand = false

@export
var _max_turn = 20.0

@export
var _turn_speed = _max_turn * 1000/100;


func _ready():
  _angle_offset = rotation_degrees

func _input(event):
  var _do_turn_previous = _do_turn
  if _use_right_hand_input:
    if event.is_action_pressed("flipper_right"):
      _do_turn = true
    elif event.is_action_released("flipper_right"):
      _do_turn = false
  else:
    if event.is_action_pressed("flipper_left"):
      _do_turn = true
    elif event.is_action_released("flipper_left"):
      _do_turn = false

  if _do_turn_check == _do_turn && _do_turn != _do_turn_previous:
    _do_turn_check = !_do_turn_check

func _physics_process(delta):
  if _do_turn_check != _do_turn:
    var _current = rotation_degrees
    var _speed = 0.0
    var _target = 0.0

    var _offset = _angle_offset
    if _is_right_hand:
      _offset *= -1
    
    if _do_turn:
      _speed = -_turn_speed
      _target = _offset - _max_turn
    else:
      _speed = _turn_speed
      _target = _offset

    var _reach = _speed * delta;
    var _dist = _target - _current
    if abs(_reach) >= abs(_dist):
      _do_turn_check = _do_turn
      _reach = _dist

      if _prevention:
        freeze = false
        freeze = true

    if _is_right_hand:
      _reach = _reach * -1

    rotation_degrees = _current + _reach

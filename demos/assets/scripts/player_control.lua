local script = {}

function script:on_init()
    Log.info("Entity " .. tostring(self.entity) .. " initialized from Lua!")
    self.speed = 200.0
    self.color_timer = 0.0
end

function script:on_update(dt)
    local transform = get_transform(self.entity)
    local pos = transform.position

    -- Movement based on input
    if Input.is_key_down(KeyCode.W) or Input.is_key_down(KeyCode.Up) then
        pos.y = pos.y + self.speed * dt
    end
    if Input.is_key_down(KeyCode.S) or Input.is_key_down(KeyCode.Down) then
        pos.y = pos.y - self.speed * dt
    end
    if Input.is_key_down(KeyCode.A) or Input.is_key_down(KeyCode.Left) then
        pos.x = pos.x - self.speed * dt
    end
    if Input.is_key_down(KeyCode.D) or Input.is_key_down(KeyCode.Right) then
        pos.x = pos.x + self.speed * dt
    end

    transform.position = pos

    -- Rotate the entity
    transform.rotation = transform.rotation + 90.0 * dt

    -- Change sprite tint over time if it has one
    if has_sprite(self.entity) then
        local sprite = get_sprite(self.entity)
        self.color_timer = self.color_timer + dt
        sprite.tint.r = (math.sin(self.color_timer) + 1.0) / 2.0
        sprite.tint.g = (math.cos(self.color_timer * 0.5) + 1.0) / 2.0
        sprite.tint.b = 1.0
    end

    -- Create a new entity when space is pressed
    if Input.is_key_pressed(KeyCode.Space) then
        Log.info("Space pressed! Creating a new entity from Lua.")
        custom_engine_call()
        local new_ent = registry:create_entity()
        add_transform(new_ent, pos.x, pos.y + 50.0)
        add_sprite(new_ent, "player.png")
        -- Note: We could also add a ScriptComponent here if we bound it
    end
end

return script

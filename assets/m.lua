function update(msgs)
    
    if #msgs == 0 then
        comp: set_active_animation("stand-right");
    else
        for i = 1, #msgs do        
            local msg = msgs[i]

            if msg == "D" and comp: get_position().x < 300 then
                comp: set_active_animation("walk-right")
                comp: set_mirror(false)
                comp: move(1, 0)
            elseif msg == "A" and comp: get_position().x > 0 then
                comp: set_active_animation("walk-right")
                comp: set_mirror(true)
                comp: move(-1, 0)
            end
        
            if msg == "W" then
                comp: jump()
            end
        end
    end
end

function ready()
    comp: add_animation("walk-right", "assets/character_right.png", 4, 5)
    comp: add_animation("stand-right", "assets/character_right_standing.png", 1, 10)
    
    comp: set_active_animation("stand-right")
end


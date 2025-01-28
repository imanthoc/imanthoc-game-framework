function update(msgs)
    for i = 1, #msgs do
        
        local msg = msgs[i];

        if msg == "D" and comp: get_position().x < 300 then
            comp: set_mirror(false);
            comp: move(1, 0);
        elseif msg == "A" and comp: get_position().x > 0 then
            comp: set_mirror(true);
            comp: move(-1, 0);
        end
        
        if msg == "W" then
            comp: jump();
        end
    end
end

% Basic code to open and display a TIFF file in MATLAB
function img = viewTiffImage(filepath)
    % Check if filepath is provided
    if nargin < 1
        [filename, pathname] = uigetfile({'*.tif;*.tiff', 'TIFF Files (*.tif, *.tiff)'}, 'Select a TIFF file');
        if isequal(filename, 0) || isequal(pathname, 0)
            disp('File selection cancelled');
            return;
        end
        filepath = fullfile(pathname, filename);
    end
    
    % Check if file exists
    if ~exist(filepath, 'file')
        error('File does not exist: %s', filepath);
    end
    
    % Read TIFF file information
    info = imfinfo(filepath);
    numFrames = numel(info);
    
    % Display file information
    fprintf('File: %s\n', filepath);
    fprintf('Number of frames: %d\n', numFrames);
    fprintf('Width: %d, Height: %d\n', info(1).Width, info(1).Height);
    fprintf('Bit depth: %d\n', info(1).BitDepth);
    
    % Read and display the image(s)
    if numFrames == 1
        % Single frame TIFF
        img = imread(filepath);
        displayImage(img, filepath);
    else
        % Multi-frame TIFF
        handleMultiframeTiff(filepath, info, numFrames);
    end
end

function displayImage(img, title_text)
    figure;
    
    % Check if image is RGB or grayscale
    if size(img, 3) == 3
        imshow(img);
    else
        % For grayscale, use appropriate display method based on bit depth
        if max(img(:)) > 255
            % Normalize for high bit depth images
            imshow(img, []);
        else
            imshow(img);
        end
    end
    
    title(title_text, 'Interpreter', 'none');
    
    % Add histogram in a subplot
    figure;
    if size(img, 3) == 3
        % RGB image - show histogram for each channel
        subplot(3,1,1);
        histogram(img(:,:,1), 'FaceColor', 'r');
        title('Red Channel');
        subplot(3,1,2);
        histogram(img(:,:,2), 'FaceColor', 'g');
        title('Green Channel');
        subplot(3,1,3);
        histogram(img(:,:,3), 'FaceColor', 'b');
        title('Blue Channel');
    else
        % Grayscale image
        histogram(img);
        title('Pixel Intensity Histogram');
    end
end

function handleMultiframeTiff(filepath, info, numFrames)
    % Create a figure for browsing multi-frame TIFF
    fig = figure('Name', 'Multi-frame TIFF Browser', 'NumberTitle', 'off');
    
    % Read first frame
    img = imread(filepath, 1);
    
    % Display first frame
    ax = axes('Parent', fig);
    if size(img, 3) == 3
        h_img = imshow(img);
    else
        if max(img(:)) > 255
            h_img = imshow(img, []);
        else
            h_img = imshow(img);
        end
    end
    
    % Add frame navigation controls
    frame_slider = uicontrol('Style', 'slider', 'Min', 1, 'Max', numFrames, ...
        'Value', 1, 'SliderStep', [1/(numFrames-1) 10/(numFrames-1)], ...
        'Position', [150 20 300 20]);
    
    frame_text = uicontrol('Style', 'text', 'Position', [50 20 90 20], ...
        'String', 'Frame: 1/' + string(numFrames));
    
    % Callback function for slider
    frame_slider.Callback = @(src, event) updateFrame(src, event, filepath, h_img, frame_text, numFrames);
    
    % Add save button
    save_button = uicontrol('Style', 'pushbutton', 'String', 'Save Current Frame', ...
        'Position', [470 20 120 20], 'Callback', @(src, event) saveCurrentFrame(filepath, round(frame_slider.Value)));
end

function updateFrame(src, ~, filepath, h_img, text_handle, numFrames)
    % Get current frame number from slider
    frameNum = round(get(src, 'Value'));
    
    % Update text display
    text_handle.String = 'Frame: ' + string(frameNum) + '/' + string(numFrames);
    
    % Read and display the selected frame
    img = imread(filepath, frameNum);
    
    % Update image
    set(h_img, 'CData', img);
    
    % Update title
    title(['Frame ' num2str(frameNum) ' of ' num2str(numFrames)]);
end

function saveCurrentFrame(filepath, frameNum)
    % Read the current frame
    img = imread(filepath, frameNum);
    
    % Ask user for save location
    [filename, pathname] = uiputfile({'*.tif;*.tiff', 'TIFF Files (*.tif, *.tiff)'; ...
                                     '*.jpg;*.jpeg', 'JPEG Files (*.jpg, *.jpeg)'; ...
                                     '*.png', 'PNG Files (*.png)'; ...
                                     '*.*', 'All Files (*.*)'}, ...
                                     'Save Frame As');
    
    if isequal(filename, 0) || isequal(pathname, 0)
        disp('Save cancelled');
        return;
    end
    
    % Save the image
    fullpath = fullfile(pathname, filename);
    imwrite(img, fullpath);
    fprintf('Frame %d saved to %s\n', frameNum, fullpath);
end

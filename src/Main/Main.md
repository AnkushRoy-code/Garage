
/// @brief the update and rendering is done in this callback function
///
/// The Update function is always running at 60fps. If for some reason it is unable to run at 60 and
/// drops below 60, the rendering has to wait. For example if update took 20ms(instead of 16).
/// Rendering will have to wait for 4ms before it starts rendering next frame.
///
/// In the other hand if rendering takes 30ms, Update will not wait. Update will go on at 16ms.
///
/// This is to prevent rendering same thing twice and keep physics based problems at minimum.

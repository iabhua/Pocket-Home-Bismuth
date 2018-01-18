/**
 * @file ConfigurableImageComponent
 * @author Anthony Brown
 * 
 * ConfigurableImageComponent defines a DrawableImageComponent with properties
 * set in the ComponentConfigFile.
 */

#pragma once
#include "ConfigurableComponent.h"
#include "../../Basic Components/DrawableImageComponent.h"

class ConfigurableImageComponent : public DrawableImageComponent,
public ConfigurableComponent {
public:
    /**
     * @param componentKey one of the component keys defined by the 
     *  ComponentConfigFile
     * @param assetIndex selects which component asset file should be loaded
     *  on initialization.
     * @param placement defines how the image will be scaled
     */
    ConfigurableImageComponent(String componentKey, int assetIndex = 0,
            RectanglePlacement placement=RectanglePlacement::centred);
    ~ConfigurableImageComponent();

    /**
     * Load a new image from a different asset file
     * @param index the index of an asset file defined for this component in
     * the ComponentConfigFile.  If index is out of bounds or it equals the
     * current loaded index, nothing will happen. 
     */
    int setImageAssetIndex(int index);
    
protected:
    /**
     * (re)load the image file and set the colors.
     * @param assetNames the list of all asset files associated with this
     * component
     * @param colours custom colour values used to replace default colours.
     */
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);

private:
    int assetIndex;
    static const Array<Colour> defaultColours;

    static const Array<Colour> loadDefaultColours() {
        Array<Colour> defaults;
        defaults.add(Colour(0xff, 0xff, 0xff));
        defaults.add(Colour(0x00, 0x00, 0x00));
        defaults.add(Colour(0xff, 0x00, 0x00));
        defaults.add(Colour(0x00, 0xff, 0x00));
        return defaults;
    };
};
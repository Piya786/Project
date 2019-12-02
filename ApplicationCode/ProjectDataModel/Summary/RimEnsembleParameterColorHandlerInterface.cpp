#include "RimEnsembleParameterColorHandlerInterface.h"

#include "RiaColorTables.h"
#include "RimRegularLegendConfig.h"
#include "RimSummaryCase.h"

#include "cvfScalarMapper.h"

#include <limits>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
namespace caf
{
template <>
void AppEnum<RimEnsembleParameterColorHandlerInterface::ColorMode>::setUp()
{
    addItem( RimEnsembleParameterColorHandlerInterface::ColorMode::SINGLE_COLOR, "SINGLE_COLOR", "Single Color" );
    addItem( RimEnsembleParameterColorHandlerInterface::ColorMode::BY_ENSEMBLE_PARAM,
             "BY_ENSEMBLE_PARAM",
             "By Ensemble Parameter" );
    setDefault( RimEnsembleParameterColorHandlerInterface::ColorMode::SINGLE_COLOR );
}
} // namespace caf

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimEnsembleParameterColorHandlerInterface::initializeLegendConfig( RimRegularLegendConfig*  legendConfig,
                                                                        const EnsembleParameter& ensembleParam )
{
    if ( ensembleParam.isText() )
    {
        std::set<QString> categories;

        for ( auto value : ensembleParam.values )
        {
            categories.insert( value.toString() );
        }

        std::vector<QString> categoryNames = std::vector<QString>( categories.begin(), categories.end() );
        legendConfig->setNamedCategories( categoryNames );
        legendConfig->setAutomaticRanges( 0, categoryNames.size() - 1, 0, categoryNames.size() - 1 );
    }
    else
    {
        double minValue = std::numeric_limits<double>::infinity();
        double maxValue = -std::numeric_limits<double>::infinity();

        for ( auto value : ensembleParam.values )
        {
            double nValue = value.toDouble();
            if ( nValue != std::numeric_limits<double>::infinity() )
            {
                if ( nValue < minValue ) minValue = nValue;
                if ( nValue > maxValue ) maxValue = nValue;
            }
        }

        legendConfig->setAutomaticRanges( minValue, maxValue, minValue, maxValue );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
cvf::Color3f RimEnsembleParameterColorHandlerInterface::caseColor( const RimRegularLegendConfig* legendConfig,
                                                                   const RimSummaryCase*         summaryCase,
                                                                   const EnsembleParameter&      ensembleParam )
{
    if ( ensembleParam.isText() )
    {
        QString tValue = summaryCase->hasCaseRealizationParameters()
                             ? summaryCase->caseRealizationParameters()->parameterValue( ensembleParam.name ).textValue()
                             : "";
        double nValue = legendConfig->categoryValueFromCategoryName( tValue );
        if ( nValue != std::numeric_limits<double>::infinity() )
        {
            int iValue = static_cast<int>( nValue );
            return cvf::Color3f( legendConfig->scalarMapper()->mapToColor( iValue ) );
        }
    }
    else
    {
        double value = summaryCase->hasCaseRealizationParameters()
                           ? summaryCase->caseRealizationParameters()->parameterValue( ensembleParam.name ).numericValue()
                           : std::numeric_limits<double>::infinity();
        if ( value != std::numeric_limits<double>::infinity() )
        {
            return cvf::Color3f( legendConfig->scalarMapper()->mapToColor( value ) );
        }
    }
    return RiaColorTables::undefinedCellColor();
}

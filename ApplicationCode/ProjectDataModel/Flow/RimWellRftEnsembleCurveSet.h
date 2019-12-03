/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RimEnsembleParameterColorHandlerInterface.h"
#include "RimSummaryCaseCollection.h"

#include "cafPdmChildField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"
#include "cvfColor3.h"

#include <QPointer>

class RiuCvfOverlayItemWidget;

class RimWellRftEnsembleCurveSet : public caf::PdmObject, public RimEnsembleParameterColorHandlerInterface
{
    CAF_PDM_HEADER_INIT;

public:
    RimWellRftEnsembleCurveSet();
    ~RimWellRftEnsembleCurveSet() override;

    RimSummaryCaseCollection* ensemble() const;
    void                      setEnsemble( RimSummaryCaseCollection* ensemble );
    ColorMode                 colorMode() const override;
    void                      setColorMode( ColorMode mode ) override;
    void                      initializeLegend();
    cvf::Color3f              caseColor( const RimSummaryCase* summaryCase ) const;
    QString                   currentEnsembleParameter() const;

    void                    setEnsembleParameter( const QString& parameterName ) override;
    std::vector<QString>    allEnsembleParameters() const;
    RimRegularLegendConfig* legendConfig() override;
    EnsembleParameter::Type currentEnsembleParameterType() const override;

protected:
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField,
                           const QVariant&            oldValue,
                           const QVariant&            newValue ) override;

    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions,
                                                         bool*                      useOptionsOnly ) override;

    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    void defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;

private:
    caf::PdmPtrField<RimSummaryCaseCollection*> m_ensemble;
    caf::PdmField<ColorModeEnum>                m_ensembleColorMode;
    caf::PdmField<QString>                      m_ensembleParameter;
    caf::PdmChildField<RimRegularLegendConfig*> m_ensembleLegendConfig;
};

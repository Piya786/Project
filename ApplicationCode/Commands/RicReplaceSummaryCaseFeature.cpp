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

#include "RicReplaceSummaryCaseFeature.h"

#include "RiaApplication.h"
#include "RiaImportEclipseCaseTools.h"
#include "RiaLogging.h"
#include "RiaSummaryTools.h"

#include "RicImportGeneralDataFeature.h"
#include "RicSummaryCaseTools.h"

#include "RicfApplicationTools.h"

#include "RimGridSummaryCase.h"
#include "RimMainPlotCollection.h"
#include "RimObservedDataCollection.h"
#include "RimObservedSummaryData.h"
#include "RimProject.h"
#include "RimSummaryCase.h"
#include "RimSummaryCaseCollection.h"
#include "RimSummaryCaseMainCollection.h"
#include "RimSummaryPlot.h"
#include "RimSummaryPlotCollection.h"

#include "cafPdmObject.h"
#include "cafSelectionManager.h"

#include <QAction>

CAF_CMD_SOURCE_INIT( RicReplaceSummaryCaseFeature, "RicReplaceSummaryCaseFeature" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicReplaceSummaryCaseFeature::isCommandEnabled()
{
    std::vector<RimSummaryCase*> caseSelection = RicSummaryCaseTools::selectedSummaryCases();

    return !caseSelection.empty();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicReplaceSummaryCaseFeature::onActionTriggered( bool isChecked )
{
    RimSummaryPlotCollection* summaryPlotColl = RiaSummaryTools::summaryPlotCollection();

    std::vector<RimSummaryCase*> caseSelection = RicSummaryCaseTools::selectedSummaryCases();
    for ( RimSummaryCase* summaryCase : caseSelection )
    {
        QString oldSummaryHeaderFilename = summaryCase->summaryHeaderFilename();

        RimGridSummaryCase* gridSummaryCase = dynamic_cast<RimGridSummaryCase*>( summaryCase );
        if ( gridSummaryCase )
        {
            const QStringList fileNames = RicImportGeneralDataFeature::getEclipseFileNamesWithDialog(
                RiaDefines::ECLIPSE_RESULT_GRID );
            if ( fileNames.isEmpty() ) return;

            const QString fileName = fileNames[0];

            RiaImportEclipseCaseTools::FileCaseIdMap fileCaseIdMap;
            bool ok = RiaImportEclipseCaseTools::openEclipseCasesFromFile( QStringList( {fileName} ),
                                                                           &fileCaseIdMap,
                                                                           true );
            if ( !ok )
            {
                QString error = QString( "Unable to load grid summary case from %1" ).arg( fileName );
                RiaLogging::error( error );
                return;
            }

            CAF_ASSERT( fileCaseIdMap.size() == 1u );

            int             caseId      = fileCaseIdMap.begin()->second;
            RimEclipseCase* eclipseCase = RicfApplicationTools::caseFromId( caseId );
            gridSummaryCase->setAssociatedEclipseCase( eclipseCase );
        }
        else
        {
            const QStringList fileNames = RicImportGeneralDataFeature::getEclipseFileNamesWithDialog(
                RiaDefines::ECLIPSE_SUMMARY_FILE );
            if ( fileNames.isEmpty() ) return;

            summaryCase->setSummaryHeaderFileName( fileNames[0] );
        }

        summaryCase->createSummaryReaderInterface();
        summaryCase->createRftReaderInterface();
        RiaLogging::info( QString( "Replaced summary data for %1" ).arg( oldSummaryHeaderFilename ) );
    }

    for ( RimSummaryPlot* summaryPlot : summaryPlotColl->summaryPlots )
    {
        summaryPlot->loadDataAndUpdate();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicReplaceSummaryCaseFeature::setupActionLook( QAction* actionToSetup )
{
    actionToSetup->setText( "Replace" );
    // TODO: get an icon?
    // actionToSetup->setIcon( QIcon( ":/Refresh-32.png" ) );
}
